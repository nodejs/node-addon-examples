/**
 * Omits a property from an object.
 *
 * @borrows https://github.com/dfee/rbx/blob/master/src/types.ts#L3
 */
export type Omit<T, K> = Pick<T, Exclude<keyof T, K>>;

/**
 * Takes two items and merges its properties. If there’s a collision, it prefers the *first* item.
 *
 * @borrows https://github.com/dfee/rbx/blob/master/src/types.ts#L3
 */
export type Prefer<P, T> = P & Omit<T, keyof P>;
