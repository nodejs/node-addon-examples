---
id: project-structure
title: Project Structure
prev: deploying
next: table-of-contents
---

The project structure of Grundgesetz is specifically designed for easy maintenance of documentation and the site's code.

```
.
|-- docs
|   |-- [path-1]
|   |   |-- [page-1].md
|   |   `-- [page-2].md
|   |-- [path-2]
|   |   |-- [page-3].md
|   |   `-- [page-4].md
|   |-- ...
|   |-- [index.md]
|   `-- toc.json
|-- [static]
|   `-- ...
`-- src
    |-- assets
    |   `-- ...
    |-- components
    |   `-- ...
    |-- layouts
    |   `-- ...
    |-- pages
    |   `-- ...
    |-- templates
    |   `-- ...
    |-- utils
    |   `-- ...
    |-- ...
    `-- typings.d.ts
```

## Explanation

- `docs/` - Where the documentation lives. Grundgesetz will read all `.md` files and builds it in a path following the directory tree.
  - `[index.md]` - An optional Index page, if you want it to be written in Markdown. This can be replaced by a customised Index page located at `pages/index.tsx`
  - `toc.json` - The file that holds the table of contents. See [Table of Contents](/writing/table-of-contents) for more information.
- `[static/]` - Holds the static assets of the site.
- `[src/]` - The root folder for the JavaScript source.
