---
id: writing-in-markdown
title: Writing in Markdown
prev: table-of-contents
next: theming-overview
---

Writing documentation in Gatsby is done in Markdown. Grundgesetz uses Remark to parse Markdown files and turn them into HTML.

## Frontmatters

We provide the following Markdown frontmatters in each documentation page.

- `id` - A unique identifier representing the markdown page. This will be referenced in `toc.json` when getting the links for the navigation sidebar, as well as the `prev` and `next` items of the pagination. **Note that the ID must be unique between pages, as this will also be used to look up the table of contents.**
- `permalink` - Optional. By default Grundgesetz will generate page paths relative to the path of the Markdown file. If you want to override the output path for a page, add this frontmatter with the **absolute** path of the output.
- `title` - The title of the page.
- `subtitle` - Optional. The subtitle of the page (if any)
- `prev` - Optional. This contains the `id` of the page that appears on the "Previous Page" navigation item.
- `next` - Optional. This contains the `id` of the page that appears on the "Next Page" navigation item.
