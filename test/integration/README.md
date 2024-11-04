# *integration* Folder

As described in the overall [OSMP Test Architecture](https://openmsl.github.io/doc/OpenMSL/test-architecture/index.html), smoke tests, integration tests and regression tests, are all considered integration tests.
This is because every model in a co-simulation depends on inputs and therefore has to be connected to at least one other FMU.
Every test folder contains a System Structure Definition file (SSD) describing the test case, a README according to the OSMP [Test Architecture Readme Template](https://github.com/openMSL/.github/blob/main/doc/integration_test_readme_template.md) and all auxiliary data needed for the test.
All tests in this folder are automatically simulated in the GitHub Action pipeline for every commit.
This folder contains some examples for different test cases, as listed below.

| File                    | Description                                                                                                                                                                                                                                   |
|-------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 001_smoke_test_tracefile          | In this smoke test the model input is connected to a trace file player. It tests, if the model can simulate the given input without any runtime errors.                                                                                       |
