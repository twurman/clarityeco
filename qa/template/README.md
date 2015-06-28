##Template

The question-answer program in template/ is a stand-alone service for use in any application.

####Basic Setup

1) Compile server: `./compile-qa.sh`

2) Start server: `./start-qa.sh (PORT)`

3) Run the tests:

```
./qaclient "who directed inception?" (PORT)
./test-concurrency-qa.sh (PORT)
```

Last Modified: 06/28/15
