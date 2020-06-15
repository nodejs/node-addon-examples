import styled from 'styled-components';
import { layout, LayoutProps, space, SpaceProps, color, ColorProps, typography, TypographyProps } from 'styled-system';

export interface TypographyProps extends LayoutProps, SpaceProps, ColorProps, TypographyProps {
  /** Extended color props. */
  color?: string;
}

/**
 * This is a base `Text` element to handle typography elements.
 */
export const Typography = styled('span')<TypographyProps>`
  ${layout}
  ${space}
  ${color}
  ${typography}
`;

Typography.displayName = 'Typography';
