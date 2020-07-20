import React from 'react';
import { graphql } from 'gatsby';
import { Helmet } from 'react-helmet';
import { RouteComponentProps } from '@reach/router';

import { Page } from 'components/layout/Page';

import { Container } from 'components/layout/Container';
import { DocsWrapper } from 'components/docs/DocsWrapper';
import { DocsHeader } from 'components/docs/DocsHeader';
import MarkdownContent from 'components/page/Markdown/MarkdownContent';

import { MenuNode, Edge } from 'interfaces/nodes';
import { Footer, FooterWrapper } from 'components/layout/Footer';
import IndexLayout from 'layouts';
import renderAst from 'utils/renderAst';
// import FooterWrapper from 'components/old-layout/FooterWrapper';
// import Footer from 'components/old-layout/Footer';

interface PageTemplateProps extends RouteComponentProps {
  data: {
    site: {
      siteMetadata: {
        title: string;
        description: string;
        author: {
          name: string;
          url: string;
        };
      };
    };
    sectionList: {
      edges: Edge<MenuNode>[];
    };
    markdownRemark: {
      htmlAst: any;
      excerpt: string;
      frontmatter: {
        id: string;
        title: string;
        prev?: string;
        next?: string;
      };
    };
  };
}

const PageTemplate: React.SFC<PageTemplateProps> = ({ data }) => {
  const { markdownRemark } = data;

  return (
    <IndexLayout>
      <Page docsPage>
        <Helmet>
          <meta property="og:title" content={markdownRemark.frontmatter.title} />
        </Helmet>
        <DocsWrapper>
          <Container>
            <DocsHeader title={markdownRemark.frontmatter.title} />
            <MarkdownContent>{renderAst(markdownRemark.htmlAst)}</MarkdownContent>
            <FooterWrapper>
              <Footer />
            </FooterWrapper>
          </Container>
        </DocsWrapper>
      </Page>
    </IndexLayout>
  );
};

export default PageTemplate;

export const query = graphql`
  query HomeTemplateQuery($slug: String!) {
    site {
      siteMetadata {
        title
        description
        siteUrl
        keywords
        author {
          name
          url
          email
        }
      }
    }
    markdownRemark(fields: { slug: { eq: $slug } }) {
      htmlAst
      excerpt
      frontmatter {
        id
        title
        prev
        next
      }
    }
  }
`;
