---
id: theming-helper-components
title: Helper Components
prev: theming-variables
---

Grundgesetz also provides helper components for you to extend the look and feel of your docs. These are essentially primitive elements with [styled-system](https://styled-system.com/) hooks attached to it so you can theme everything easily according to your branding.

## Reset

Provides global CSS resets for grundgesetz-skeleton.

### Usage

Wrap your index layout inside the `ThemeReset` to use the resets and `ThemeProvider` to be used by `styled-system`.

```jsx
import { ThemeReset } from 'path/to/components/foundations';

export default function MyApp({ children }) {
  return <ThemeReset>{children}</ThemeReset>;
}
```

You can also use `GlobalStyles` as a standalone component which injects the base styles into the component.

```jsx
import { GlobalStyles, Theme } from 'path/to/components/foundations';

export default function MyApp({ children }) {
  return (
    <Theme>
      <GlobalStyles />
      {children}
    </Theme>
  );
}
```

## Box

Box is a primitive element with all `styled-system` hooks attached to it.

### Usage

Basic example:

```jsx
import { ThemeReset, Box } from 'path/to/components/foundations';

export default function MyApp({ children }) {
  return (
    <ThemeReset>
      <Box>I'm inside a box!</Box>
    </ThemeReset>
  );
}
```

Box renders a `div` by default. You can use the `as` prop to change the element it renders

```jsx
import { Box } from 'path/to/components/foundations';

export default function MyComponent({ children }) {
  return <Box as="header">I'm inside a box with a header tag!</Box>;
}
```

You can use a lot of `styled-system` hooks with the `Box` component. For example:

```jsx
import { Box } from 'path/to/components/foundations';

export default function MyComponent({ children }) {
  return (
    <Box color="white" bg="blue" p="md">
      I'm inside a box with padding!
    </Box>
  );
}
```

You can even use it to compose an entire component:

```jsx
import { Box, Heading, Text } from 'path/to/components/foundations';

export default function DocsHeader({ title, subtitle }) {
  return (
    <Box as="header" mb="md">
      <Heading as="h1" scale={800} color="grey09">
        {title}
      </Heading>
      {subtitle && (
        <Text as="p" scale={400} mt="md">
          {subtitle}
        </Text>
      )}
    </Box>
  );
}
```

## Typography

Typography primitives provided by grundgesetz-skeleton.

### Usage

**Heading**

The heading component is used for all headings. All typography components provide a `scale` prop to determine typography scale (font-size, line-height, and letter-spacing) based on the size token.

```jsx
import { Heading } from 'path/to/components/foundations';

export default function MyConponent({ children }) {
  return <Heading scale={900}>We are making AI technology accessible to everyone</Heading>;
}
```

Heading renders a `h2` by default. If you would like to determine which HTML element it renders, you can use the `as` prop.

```jsx
<Heading scale={700} as="h3">
  We are making AI technology accessible to everyone
</Heading>
```

**Text**

The text component is used for single line text.

```jsx
import { Text } from 'path/to/components/foundations';

export default function MyComponent({ children }) {
  return <Text scale={400}>We are making AI technology accessible to everyone</Text>;
}
```

Text renders a `span` by default. If you would like to determine which HTML element it renders, you can use the `as` prop.

```jsx
<Text size={300} as="p">
  We are making AI technology accessible to everyone
</Heading>
```

**Paragraph**

Renders a paragraph-sized text by default

```jsx
import { Paragraph } from 'path/to/components/foundations';

export default function MyComponent() {
  return (
    <Paragraph>
      We are Kata.ai, an Indonesian conversational Artificial Intelligence company, focused on understanding human
      conversation so we can improve the way humans collaborate with technology to be more productive and empowered.
      Kata.ai’s Natural Language Processing (NLP) technology powers multi-purpose chatbots for major corporations in
      Indonesia across different industries, including FMCG, Telecommunication, Banking & Financial Service, and Retail.
    </Paragraph>
  );
}
```
