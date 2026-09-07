@echo OFF
..\x64\Release\SpiderSolverMain.exe --limitSeconds 100 --randomSeeds 100-299 --suits 1 --treeDepths 2-6 
..\x64\Release\SpiderSolverMain.exe --limitSeconds 100 --randomSeeds 100-299 --suits 2 --treeDepths 2-6 | findstr /v suits
..\x64\Release\SpiderSolverMain.exe --limitSeconds 100 --randomSeeds 100-299 --suits 4 --treeDepths 2-6 | findstr /v suits
