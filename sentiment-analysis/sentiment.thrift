
struct Response {
	1: string score,
	2: string polarity
}

service SentimentAnalysis {
	string doAnalysis(1: string content)
}
