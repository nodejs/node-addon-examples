---
id: table-of-contents
title: Table of Contents
prev: project-structure
next: writing-in-markdown
---

The `toc.json` file inside the `docs/` folder holds our table of contents. It will be automatically populated on the left-side drawer as a sectioned menu.

```json
[
  {
    "title": "Menu Section 1",
    "items": [
      {
        "id": "page-1",
        "slug": "/section-1/page-1",
        "title": "Page 1"
      },
      {
        "id": "page-2",
        "slug": "/section-1/page-2",
        "title": "Page 2"
      },
      {
        "id": "page-3",
        "slug": "/section-1/page-3",
        "title": "Page 3"
      }
    ]
  },
  {
    "title": "Menu Section 2",
    "items": [
      {
        "id": "page-1",
        "slug": "/section-2/page-1",
        "title": "Page 1"
      },
      {
        "id": "page-2",
        "slug": "/section-2/page-2",
        "title": "Page 2"
      },
      {
        "id": "page-3",
        "slug": "/section-2/page-3",
        "title": "Page 3"
      }
    ]
  }
]
```
