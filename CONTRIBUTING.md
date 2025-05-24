# Contributing to Reflect
Before you start, we would like to tell you that this project has some differences on coding conventions, even though its based on the LLVM coding convention. 

Here is our coding convention:
- 2 spaces for whitespace (no tabs)
- camelCase for functions/methods
- camelCase for variables
- camelCase for fields
- Fields do not have the `m_` prefix
- PascalCase for class/struct names
- PascalCase for typedefs
- Lowercase for namespace names
- UPPER_SNAKE_CASE for macros

It's really just LLVM, but with some slight differences.
Alright, now let's get to the main part.

*Thank you for contributing!*
## Bugs
- Please check for issues regarding your bug. If you have found one, do not open a new issue.

- Provide a concise title and it should be prefixed with `[BUG]: `.

- Provide also a clear description, and fill in the blanks if necessary.

- If it is a security vulnerability, please also prefix `[VULNERABILITY]` behind the other prefix like `[VULNERABILITY] [BUG]: `.

- You should include as much information as possible, and other things (logs, breakpoint statements, etc.) if possible. And be sure to provide the steps to reproduce it, as well optionally to attach the patched executable (if you can build it, of course)

- Be sure to use the bug templates, as it can lead to your issue being resolved faster. 
## Feature requests

- Again, please check for issues related to your request. If you found one, please do not open a new one.

- Provide a concise title with a detailed description if necessary. Please also prefix `[FEATURE REQUEST]: ` to your issue.

- If the feature request is purely cosmetic and does not change the overall codebase, please do not open a new issue. Most likely, we may resolve it soon. If not, contact the developers about this.

- The feature request must include the public API and other technical things to ensure compliance with your issue.

- You should also attach logs and other info as well as the modified executable and source code you deem satisfactory.
## Documentation

- If you want to contribute to the documentation (either code documentation or general), you can open a new issue.

- Provide a detailed description, and be sure to prefix `[DOCUMENTATION]:` into your title.

- We are very relaxed on documentation. As long as it is not against the code of conduct and is reviewed, you are good to go.

