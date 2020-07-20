import React from 'react';
import styled from 'styled-components';
import { colors } from 'components/foundations/variables';
import { Paragraph } from 'components/foundations';

const Wrapper = styled('footer')`
  padding-top: 24px;
  border-top: 1px solid ${colors.grey02};
`;

const Footer: React.SFC = () => (
  <Wrapper>
    <Paragraph margin={0}>
      <div>
        Comments or suggestions about this page?{' '}
        <a href="https://github.com/nodejs/node-addon-examples/issues" target="_blank" rel="noopener noreferrer">
          Let us know
        </a>
        .
      </div>
      <small>
        Created with{' '}
        <a href="https://github.com/kata-ai/grundgesetz-skeleton" target="_blank" rel="noopener noreferrer">
          Grundgesetz
        </a>
        .
      </small>
    </Paragraph>
  </Wrapper>
);

export default Footer;
