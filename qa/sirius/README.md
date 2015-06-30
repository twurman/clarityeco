##Sirius

The question-answer program in sirius/ belongs to a collection of services that connect with Sirius.

####Basic Setup
If Sirius is not already running, first follow the directions [here](../../command-center/README.md).

1) Compile server: `./compile-qa.sh`

2) Start server: `./start-qa.sh (PORT) (SIRIUS-PORT)`

3) Run the tests:

```
cd ../../command-center
./ccclient --qa (SIRIUS-PORT)
```

Last Modified: 06/28/15
