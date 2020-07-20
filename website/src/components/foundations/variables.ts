/**
 * Example color values. The default here uses the colours used by the Aksara
 * design system. You can modify these according to your needs.
 */
export const colors = {
  // Blue
  blue01: '#e7f1fc',
  blue02: '#b9d7f8',
  blue03: '#8bbdf3',
  blue04: '#5ca3ef',
  blue05: '#2e89ea',
  blue06: '#006fe6',
  blue07: '#005bbd',
  blue08: '#004793',
  blue09: '#003369',
  blue10: '#001f3f',

  // Indigo
  indigo01: '#e7eaf4',
  indigo02: '#b9c0df',
  indigo03: '#8b97c9',
  indigo04: '#5c6db4',
  indigo05: '#2e449f',
  indigo06: '#001b8a',
  indigo07: '#001771',
  indigo08: '#001258',
  indigo09: '#000d3f',
  indigo10: '#000826',

  // Turquoise
  turquoise01: '#e7fafd',
  turquoise02: '#b9f1f9',
  turquoise03: '#8be7f5',
  turquoise04: '#5cdef1',
  turquoise05: '#2ed5ed',
  turquoise06: '#00cce9',
  turquoise07: '#00a7bf',
  turquoise08: '#008295',
  turquoise09: '#005d6a',
  turquoise10: '#003840',

  // Green
  green01: '#f2f8f0',
  green02: '#daecd3',
  green03: '#c1e0b7',
  green04: '#a9d49a',
  green05: '#90c87d',
  green06: '#78bc61',
  green07: '#639a50',
  green08: '#4d783e',
  green09: '#37562d',
  green10: '#21341b',

  // Yellow
  yellow01: '#fff7ed',
  yellow02: '#ffe7ca',
  yellow03: '#ffd7a8',
  yellow04: '#ffc885',
  yellow05: '#ffb862',
  yellow06: '#ffa940',
  yellow07: '#d18b35',
  yellow08: '#a36c29',
  yellow09: '#744d1e',
  yellow10: '#462f12',

  // Red
  red01: '#fce9e8',
  red02: '#f7bfbc',
  red03: '#f2958f',
  red04: '#ed6b63',
  red05: '#e84136',
  red06: '#e3170a',
  red07: '#ba1309',
  red08: '#910f07',
  red09: '#680b05',
  red10: '#3e0703',

  // Grey
  grey01: '#fafafa',
  grey02: '#edeeee',
  grey03: '#cacece',
  grey04: '#858e8d',
  grey05: '#626e6d',
  grey06: '#404e4d',
  grey07: '#293232',
  grey08: '#1e2423',
  grey09: '#121615',
  grey10: '#060807',

  // Helper colors
  white: '#fff',
  black: '#000'
};

export const systemFonts =
  "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif, 'Apple Color Emoji', 'Segoe UI Emoji', 'Segoe UI Symbol'";

export const fonts = {
  system: systemFonts,
  // Example for including additional fonts to the default sans-serif stack.
  sansSerif: `Barlow, ${systemFonts}`,
  monospace: "'SF Mono', Inconsolata, Menlo, Monaco, Consolas, 'Courier New', Courier, monospace;"
};

/** Heading size values mapped by size number. */
export const headingSizes = {
  900: {
    fontSize: 42,
    lineHeight: 48,
    letterSpacing: -0.2
  },
  800: {
    fontSize: 35,
    lineHeight: 40,
    letterSpacing: -0.2
  },
  700: {
    fontSize: 29,
    lineHeight: 32,
    letterSpacing: -0.2
  },
  600: {
    fontSize: 24,
    lineHeight: 28,
    letterSpacing: -0.05
  },
  500: {
    fontSize: 20,
    lineHeight: 24,
    letterSpacing: -0.05
  },
  400: {
    fontSize: 16,
    lineHeight: 20,
    letterSpacing: -0.05
  },
  300: {
    fontSize: 14,
    lineHeight: 20,
    letterSpacing: -0.05
  },
  200: {
    fontSize: 12,
    lineHeight: 16,
    letterSpacing: 0
  },
  100: {
    fontSize: 12,
    lineHeight: 16,
    letterSpacing: 0.5
  }
};

/** Text size values mapped by size number. */
export const textSizes = {
  500: {
    fontSize: 20,
    lineHeight: 24
  },
  400: {
    fontSize: 16,
    lineHeight: 20
  },
  300: {
    fontSize: 14,
    lineHeight: 20
  },
  200: {
    fontSize: 12,
    lineHeight: 16
  }
};

/** Text size values mapped by size number. */
export const paragraphSizes = {
  400: {
    fontSize: 16,
    lineHeight: 24
  },
  300: {
    fontSize: 14,
    lineHeight: 24
  }
};

/** Space values (in px) mapped by size designators */
export const space = {
  /** Equivalent to 2px */
  xxxs: 2,
  /** Equivalent to 4px */
  xxs: 4,
  /** Equivalent to 8px */
  xs: 8,
  /** Equivalent to 12px */
  sm: 12,
  /** Equivalent to 16px */
  md: 16,
  /** Equivalent to 24px */
  lg: 24,
  /** Equivalent to 32px */
  xl: 32,
  /** Equivalent to 48px */
  xxl: 48
};

/** Breakpoint values (in px) mapped by size designators */
export const breakpoints = {
  /** 0px to 319px */
  xs: 0,
  /** 320px to 767px */
  sm: 320,
  /** 768px to 1023px */
  md: 768,
  /** 1024px to 1439px */
  lg: 1024,
  /** 1440px and above */
  xl: 1440
};

export const layerIndexes = {
  base: 0,
  flat: 1,
  floating: 2,
  stickyNav: 8,
  overlay: 16,
  dialog: 24,
  popout: 32
};

export const dimensions = {
  widths: {
    sidebar: {
      sm: 240,
      md: 280,
      lg: 200
    },
    containerPadding: space.lg
  },
  heights: {
    header: 64
  }
};
