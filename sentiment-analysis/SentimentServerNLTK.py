# -*- coding: utf-8 -*-
"""
This sentiment analysis code is initially adapted by: Chang-Hong Hsu (hsuch@umich.edu).Thrift support as well as logging is added to transform into a service by: Hailong Yang (hailong@umich.edu). Current version is using NLTK to perform POS of the sentences.

"""
import nltk
import yaml
import sys, glob
import logging
sys.path.append('gen-py')
sys.path.insert(0, glob.glob('lib/py/lib.*')[0])

import os
import re
from parse import *
from collections import defaultdict
import datetime
from datetime import timedelta

from sentiment import SentimentAnalysis
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TJSONProtocol
from thrift import TSerialization;
from thrift.server import TServer
from sentiment.ttypes import *

class SentimentAnalysisHandler:
    def __init__(self):
        self.splitter = Splitter()
        self.postagger = POSTagger()
        self.dicttagger = DictionaryTagger()
        self.dicttagger.parse_shifter_dictionaries(['dicts/inc.yml', 'dicts/dec.yml', 'dicts/inv.yml'])
        self.dicttagger.parse_polarity_dictionary('dicts/subjclueslen1-HLTEMNLP05.tff')
	logging.basicConfig(filename='service.log', format='[%(asctime)s]%(levelname)s: %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p', level=logging.INFO)

    def doAnalysis(self, content):
	logging.info('received original content - ' + content.strip())
	clean_content = preprocess(content)
	logging.info('content after cleaning - ' + clean_content)
        t_begin = datetime.datetime.now()
        splitted_sentences = self.splitter.split(clean_content)
        pos_tagged_sentences = self.postagger.pos_tag(splitted_sentences)
        dict_tagged_sentences = self.dicttagger.tag(pos_tagged_sentences)

        score = sentiment_score(dict_tagged_sentences)
        t_end = datetime.datetime.now()
        t_delta = t_end - t_begin
        t_delta_milliseconds = float(t_delta.days * 86400 * 1000000 + t_delta.seconds * 1000000 + t_delta.microseconds)/1000.0
	analysis = Response()
	if score > 0:
		analysis.polarity = 'positive'
	elif score < 0:
		analysis.polarity = 'negative'
	else:
		analysis.polarity = 'neutral'
	analysis.score = str(score)
	json = TSerialization.serialize(analysis, TJSONProtocol.TSimpleJSONProtocolFactory());
        logging.info('sentiment analysis score: %.1f' % score)
	logging.info('replied with JSON format: ' + json)
        logging.info("elapsed time for analysis = {0} ms".format(t_delta_milliseconds) + '\n')
	return json

class Splitter(object):

    def __init__(self):
        self.nltk_splitter = nltk.data.load('tokenizers/punkt/english.pickle')
        self.nltk_tokenizer = nltk.tokenize.TreebankWordTokenizer()

    def split(self, text):
        sentences = self.nltk_splitter.tokenize(text)
        tokenized_sentences = [self.nltk_tokenizer.tokenize(sent) for sent in sentences]
        return tokenized_sentences


class POSTagger(object):

    def __init__(self):
	pass
        
    def pos_tag(self, sentences):
        pos = [nltk.pos_tag(sentence) for sentence in sentences]
        pos = [[(word, word, [postag]) for (word, postag) in sentence] for sentence in pos]
        return pos

class DictionaryEntry(object):
    def __init__(self, line):
        r = parse("type={type} len={len} word1={word} pos1={pos} stemmed1={stemmed} priorpolarity={polarity}", line)
        self.prop = {}
        for p in ['type', 'len', 'word', 'pos', 'stemmed', 'polarity']:
            self.prop[p] = r[p]

class DictionaryTagger(object):
    def __init__(self):
        pass

    def parse_shifter_dictionaries(self, dictionary_paths):
        files = [open(path, 'r') for path in dictionary_paths]
        dictionaries = [yaml.load(dict_file) for dict_file in files]
        map(lambda x: x.close(), files)
        self.shifter_dictionary = {}
        for curr_dict in dictionaries:
            for key in curr_dict:
                key_str = str(key)
                if key_str in self.shifter_dictionary:
                    self.shifter_dictionary[key_str].extend(curr_dict[key_str])
                else:
                    self.shifter_dictionary[key_str] = curr_dict[key_str]

    def parse_polarity_dictionary(self, dictionary_path):
        self.polarity_dictionary = defaultdict(dict)
        self.max_key_size = 0
        dictionary_file = open(dictionary_path, 'r')
        for line in dictionary_file:
            entry = DictionaryEntry(line)
            key_str = str(entry.prop['word'])
            sub_key_str = str(entry.prop['pos'])
            if key_str in self.polarity_dictionary:
                if sub_key_str in self.polarity_dictionary[key_str]:
                    continue
                else:
                    self.polarity_dictionary[key_str][sub_key_str] = entry
            else:
                self.polarity_dictionary[key_str] = {sub_key_str: entry}
        dictionary_file.close()
   

    def tag(self, postagged_sentences):
        return [self.tag_sentence(sentence) for sentence in postagged_sentences]

    def tag_sentence(self, sentence, tag_with_lemmas=False):
        tag_sentence = []
        N = len(sentence)
        if self.max_key_size == 0:
            self.max_key_size = N
        i = 0
        while ( i < N ):
            j = min(i + self.max_key_size, N)
            tagged = False
            shifter_tagged = False
            
            while ( j > i ):
                expression_form = ' '.join([word[0] for word in sentence[i:j]]).lower()
                expression_lemma = ' '.join([word[1] for word in sentence[i:j]]).lower()
                if tag_with_lemmas:
                    literal = expression_lemma
                else:
                    literal = expression_form

                if literal in self.polarity_dictionary:
                    is_single_token = j - i == 1
                    original_position = i
                    i = j
                    pos_tag = sentence[original_position][2]
                    taggings = [tag for tag in self.lookup_with_pos_tag(literal, pos_tag)]
                    tagged_expression = (expression_form, expression_lemma, taggings)
                    if is_single_token:
                        original_token_tagging = sentence[original_position][2]
                        tagged_expression[2].extend(original_token_tagging)
                    tag_sentence.append(tagged_expression)
                    tagged = True

                if literal in self.shifter_dictionary:
                    is_single_token = j - i == 1
                    original_position = i
                    i = j
                    taggings = [tag for tag in self.lookup_shifter(literal)]
                    tagged_expression = (expression_form, expression_lemma, taggings)
                    if is_single_token:
                        original_token_tagging = sentence[original_position][2]
                        tagged_expression[2].extend(original_token_tagging)
                    tag_sentence.append(tagged_expression)
                    shifter_tagged = True
                else:
                    j = j - 1
            if not tagged and not shifter_tagged:
                tag_sentence.append(sentence[i])
                i += 1
        return tag_sentence

    def lookup_with_pos_tag(self, token, pos_tag):
        if token in self.polarity_dictionary:
            entry = None
            if pos_tag in ['JJ', 'JJR', 'JJS']:
                #adjective
                entry = self.polarity_dictionary[token]['adj']
            elif pos_tag in ['RB', 'RBR', 'RBS']:
                #adverb
                entry = self.polarity_dictionary[token]['adverb']
            elif pos_tag in ['NN', 'NNS', 'NNP']:
                #noun
                entry = self.polarity_dictionary[token]['noun']
            elif pos_tag in ['VB', 'VBD', 'VBG', 'VBN', 'VBP', 'VBZ']: 
                entry = self.polarity_dictionary[token]['verb']
            else:
                if 'anypos' in self.polarity_dictionary[token]:
                    entry = self.polarity_dictionary[token]['anypos']
            if entry is not None:
                return [entry.prop['polarity'], entry.prop['type']]
            else:
                return []
        else:
            return []
    def lookup_shifter(self, token):
        return [shifter_tag for shifter_tag in self.shifter_dictionary[token]]

def value_of_shifter(tags):

    if 'inv' in tags:
        return -1.0
    elif 'dec' in tags:
        return 0.5
    elif 'inc' in tags:
        return 2.0
    else:
        return 1.0

def value_of_sentiment(tags):
    base_score = 0.0
    if 'positive' in tags:
        base_score = 1.0
    elif 'negative' in tags:
        base_score = -1.0
    elif 'neutral' in tags:
        base_score = 0.0
    if 'weaksubj' in tags:
        base_score *= 1.0
    elif 'strongsubj' in tags:
        base_score *= 2.0
    return base_score

def sentence_score(sentence_tokens, previous_token, acum_score):    
    if not sentence_tokens:
        return acum_score
    else:
        current_token = sentence_tokens[0]
        tags = current_token[2]
        token_score = value_of_sentiment(tags)
        if previous_token is not None:
            previous_tags = previous_token[2]
            modifier = value_of_shifter(previous_tags)
            token_score *= modifier
        return sentence_score(sentence_tokens[1:], current_token, acum_score + token_score)

def sentiment_score(review):
    summation = 0.0
    for sentence in review:
        score = sentence_score(sentence, None, 0.0)
        summation += score
    return summation

def preprocess(content):
    # remove the extra whitespaces
    clean_content = ''
    for word in content.strip().split():
        # remove none sense http links
        if 'http' not in word:
            clean_content += word + ' '
    return clean_content

service_port = 9090
if len(sys.argv) == 2:
	service_port = int(sys.argv[1])
	print 'Changing the service port to %d' % service_port
else:
	print 'Service port is not setup, using the default %d' % service_port

handler = SentimentAnalysisHandler()
processor = SentimentAnalysis.Processor(handler)
transport = TSocket.TServerSocket(port=service_port)
tfactory = TTransport.TBufferedTransportFactory()
pfactory = TJSONProtocol.TJSONProtocolFactory()
server = TServer.TThreadPoolServer(processor, transport, tfactory, pfactory)
print 'Starting the sentiment server...'
server.serve()
print 'done.'

