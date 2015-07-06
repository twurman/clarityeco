# Sentiment Analysis Service

This is a python based sentiment analysis service integrating with Apache Thrift  running as part of the clarity ecosystem. It accepts natural language text and calculates the polarity and score for the emotion behind the text.(e.g. positive or negative, as well as how strong it is)

## Getting started

### Prerequisites

* Python(2.7 or later)
* [Apache Thrift](https://thrift.apache.org/)(0.9 or later)
  - sudo pip install thrift
* [Natural Language Toolkit](http://www.nltk.org/)(3.0)
  - sudo pip install -U nltk
  - download the NLTK data(from python interactive shell)
      - import nltk
      - nltk.download()
* (optional) [Senna](http://ml.nec-labs.com/senna/)(3.0)

### Generate the thrift glue codes

The thrift interfaces are already defined in the .thrift files. Type the following commands to generate the glue codes for your favorite language. Refer to the [Apache Thrift](https://thrift.apache.org/) for more information.

$ thrift --gen py sentiment.thrift

### Start the Sentiment Analysis Service

* NLTK version
  - Usage: python SentimentServerNLTK.py [port]
* Senna version
  - Usage: python SentimentServerSENNA.py PATH-TO-SENNA [port]

### Start the Sentiment Analysis Client

Usage: python SentimentClient.py CONTENT 

### Sample Input

Put the following sentences into a file and try it out with the sentiment analysis.

"This is an awesome tool for sentiment analysis. I love it very much."

### Happy End
Have fun and enjoy!
