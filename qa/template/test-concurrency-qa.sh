# script to send multiple client requests in parallel
# Usage: ./test-concurrency.sh <PORT>
./qaclient --factoid "what is the speed of light?" $1 &
./qaclient --factoid "who is the President of the United States?" $1 &
./qaclient --factoid "who wrote Harry Potter?" $1 &
