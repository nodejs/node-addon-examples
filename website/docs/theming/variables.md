---
id: theming-variables
title: Variables
prev: theming-overview
next: theming-helper-components
---

Since Grundgesetz utilises [styled-components](https://styled-components.com/) and [styled-system](https://styled-system.com/) under the hood, changing the look and feel of it is as simple as changing variables inside the [variables file](https://github.com/kata-ai/grundgesetz-skeleton/blob/master/src/components/foundations/variables.ts). We've provided some sample variables for you to play around, so you won't get overwhelmed.

## Available Variables

These are the essential variables that you can play around with. There are more variables that are provided outside of the ones listed below, so don't be afraid to peek around the [variables file](https://github.com/kata-ai/grundgesetz-skeleton/blob/master/src/components/foundations/variables.ts) and the [default theme provider](https://github.com/kata-ai/grundgesetz-skeleton/blob/master/src/components/foundations/Theme.tsx).

### Colors

Available color tokens for your docs. This will be mapped into the `ThemeProvider` as a `styled-system` [color prop](https://styled-system.com/table#color). By default we provided a bunch of colour options for you to get started. If you want to replace the colors with your own, feel free to do so:

```js
export const colors = {
  white: '#FFF',
  black: '#000',
  danger: '#F00'
};
```

The color key can then be referenced as a prop in any color-related styled-system prop, for example:

```jsx
import { Box } from 'components/foundations';

function SomeComponent() {
  return (
    <Box color="white" bgColor="danger">
      Alert!
    </Box>
  );
}
```

### Fonts

Sets your application's font stack, which will be mapped into `styled-system`'s [typography prop](https://styled-system.com/table/#typography). We provide 3 font keys, `system` (for system-based fonts), `sansSerif` (for extending system fonts with your own additional font), and `monospace` (for code blocks).

```js
export const fonts = {
  system: systemFonts,
  // Example for including additional fonts to the default sans-serif stack.
  sansSerif: `Barlow, ${systemFonts}`,
  monospace: "'SF Mono', Inconsolata, Menlo, Monaco, Consolas, 'Courier New', Courier, monospace;"
};
```

```jsx
import { Heading } from 'components/foundations';

function SomeComponent() {
  return <Heading fontFamily="sansSerif">Example Heading</Heading>;
}
```

### Spacing

Spacing tokens for margins and padding, mapped into `styled-system`'s [space prop](https://styled-system.com/table#space).

```js
export const space = {
  xxxs: 2,
  xxs: 4,
  xs: 8,
  sm: 12,
  md: 16,
  lg: 24,
  xl: 32,
  xxl: 48
};
```

```jsx
import { Box } from 'components/foundations';

function SomeComponent() {
  return (
    <Box color="white" bgColor="blue" p="md">
      I'm inside a box!
    </Box>
  );
}
```
