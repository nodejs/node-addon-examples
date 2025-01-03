export declare class AsyncIteratorExample {
    from: number;
    to: number;
    constructor(from: number, to: number);
    [Symbol.asyncIterator](): {
        current: number;
        last: number;
        next(): Promise<unknown>;
    };
}
