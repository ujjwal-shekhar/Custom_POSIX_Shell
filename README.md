[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description

# Assumptions
- Assumed that the file format for history will be `.log`
- Assumed that during the vivas an empty `pastevents.log` file will be manually created by either the TA or me in the `pasteventsHandler/` directory. 
- Assumed that I dont have to handle stuff like `COMMAND &> FILE`.
- Assuming that `warp` is equal to `warp ~`

# Usage of AI tools
- The `utils` directory is almost entirely my idea and the only help that AI tools did there was to auto complete some of the grunt code
- `prompt.c` was mostly written by me with ome help to figure out the `hostname` from ChatGPT.
- The 4-function structure of `pastevents.c` was mostly written by me with added help in correction of some erroneous code and file handling in the `history` array.
- In `peek.c`, the `-l` flag implementation and the sorting by lexicographic order was suggested by ChatGPT and I had also confirmed from stackoverflow articles that it is infact correct.
- The implementation for `seek.c` was written by me while referring to most of the `peek.c` code. However, the DFS over directory DAG was written by me completely (with maybe Copilot autcompleting repetitive lines).
- Any code that is handling the `bg` process, is a mix of many online blog posts, me, and ChatGPT.
- Any declarations not here can be found in the individual files.
- `peek -la` and `peek -al` will be considered different when storing in the `pastevents.log`.