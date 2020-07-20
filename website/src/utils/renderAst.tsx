import * as React from 'react';
import rehypeReact from 'rehype-react';
import { h1, h2, h3, h4, h5, h6, p, ul, li, table } from 'components/page/Markdown/MarkdownComponents';

export interface ComponentMap {
  [key: string]: React.ComponentType<any>;
}

export function renderAst(markdownAst: any, additionalComponents: ComponentMap = {}) {
  const rehype = new rehypeReact({
    createElement: React.createElement,
    components: {
      h1,
      h2,
      h3,
      h4,
      h5,
      h6,
      p,
      ul,
      li,
      table,
      ...additionalComponents
    }
  });
  return rehype.Compiler(markdownAst);
}

export default renderAst;
