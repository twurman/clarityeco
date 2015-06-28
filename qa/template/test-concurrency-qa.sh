# script to send multiple client requests in parallel
./qaclient "what is the speed of light?" $1 &
./qaclient "who is the President of the United States?" $1 &
./qaclient "what is the speed of sound?" $1 &
./qaclient "who wrote Harry Potter?" $1 &
