***This is a final project in "Skillbox" at programm "C++ developer"***

***Stack:***
In this graduate work used Nils Nlohman's library "JSON for modern C++" for parse .JSON files.
For unit-tests i used Gtets packege from Google (some issued tests have been reworked).
Program use smart-pointers and auto created threads for better speed work. 
All project write in Ms Visual Studio IDE (for better compatibility it is better to use it).

***How used:***

1. download the project and build it in your IDE;

2. Create folders in the root folder of the project assembly: "resouses" - documents for indexing will be placed in it,
"config" - in it you need to create the files "config.json" and "requests.json" (the purpose and examples of filling will be described below);
3. Fill in the “config” (program operation configuration) and “requests” (list of search queries) files using the following examples:

***An example of a config.json file description:***

{
  "config": {
  "name": "SkillboxSearchEngine",
  "version": "0.1",
  "max_responses": 5
  },
  "files": [
  "../resources/file001.txt",
  "../resources/file002.txt",
  "../resources/file003.txt",
  "../resources/file004.txt",
  …
  ]
}

***An example of a requests.json file description:***

{
  "requests": [
  "some words..",
  "some words..",
  "some words..",
  "some words..",
  ...
  ]
}

4. ***Launch the program*** using the internal menu

