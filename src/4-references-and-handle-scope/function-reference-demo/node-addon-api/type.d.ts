export declare class NativeAddon {
    constructor(JSFnRef: () => void, JSFn: () => void);
    /**
     * call the JSFn by the stored reference
     */
    tryCallByStoredReference(): void;
    /**
     * call the JSFn directly
     */
    tryCallByStoredFunction(): void;
}