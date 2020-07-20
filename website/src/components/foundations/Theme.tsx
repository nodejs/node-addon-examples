import * as React from 'react';
import { ThemeProvider } from 'styled-components';
import { breakpoints, headingSizes, textSizes, colors, space, fonts, paragraphSizes } from './variables';

export const themeProps = {
  colors,
  space,
  fonts,
  breakpoints: [`${breakpoints.sm}px`, `${breakpoints.md}px`, `${breakpoints.lg}px`, `${breakpoints.xl}px`],
  typeScale: {
    heading: headingSizes,
    paragraph: paragraphSizes,
    text: textSizes
  }
};

export const Theme = (props: { children: React.ReactNode }) => {
  return (
    <ThemeProvider theme={themeProps}>
      <>{props.children}</>
    </ThemeProvider>
  );
};

export type TypeScale = typeof themeProps.typeScale;
export type HeadingSizes = typeof headingSizes;
export type TextSizes = typeof textSizes;
export type Color = keyof typeof themeProps['colors'];
export type Space = keyof typeof themeProps['space'];
