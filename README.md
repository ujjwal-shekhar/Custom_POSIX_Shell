# Description

# Assumptions
- The command won't be **cleaned** before storing in `pastevents.log`. That is
```bash
warp                       ~
```

will still be stored in `pastevents.log` as 
```bash
warp                       ~
```

and **NOT** as 
```bash
warp ~
```

- Assumed that the file format for history will be `.log`
- Assumed that t