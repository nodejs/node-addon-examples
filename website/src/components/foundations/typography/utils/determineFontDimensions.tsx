import { themeProps, TypeScale } from '../../Theme';

/**
 * Determines font sizes based on the text type and size index.
 *
 * @param textType Either `text` or `heading`.
 * @param scale The size key.
 */
export function determineFontDimensions(textType: keyof TypeScale, scale: number = 400) {
  const match: any = (themeProps.typeScale[textType] as any)[scale];

  if (textType === 'heading') {
    const styleProps = {
      fontSize: `${match.fontSize}px`,
      lineHeight: `${match.lineHeight}px`,
      fontWeight: scale <= 400 ? 600 : 500,
      letterSpacing: `${match.letterSpacing}px`
    };

    return {
      ...styleProps,
      ...(scale === 100 ? { textTransform: 'uppercase' } : {})
    };
  }

  return {
    fontSize: `${match.fontSize}px`,
    lineHeight: `${match.lineHeight}px`,
    fontWeight: 400
  };
}
