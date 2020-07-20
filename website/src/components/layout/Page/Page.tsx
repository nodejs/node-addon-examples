import styled from 'styled-components';
import { dimensions } from 'components/foundations/variables';

interface PageProps {
  docsPage?: boolean;
}

const Page = styled('main')<PageProps>`
  display: flex;
  flex-direction: column;
  flex: 1 1 auto;
  position: relative;
  margin-top: ${dimensions.heights.header}px;
  padding: 0;
`;

export default Page;
