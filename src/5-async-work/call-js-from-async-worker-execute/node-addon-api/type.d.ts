type Action = 
| ((action: "open" | "close") => void)
| ((action: "message", data: string) => void)
| ((action: "error", data: Error) => void);

export function dispatch(callback: Action): void;
