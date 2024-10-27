# SOARE

## INTERPRETER

```soare
try:
    loadimport "script.soare";
iferror:
    echo `"script.soare" is required.`;
    exit;
close;

function Number main(Array args):

    datas: ["", 0];

    datas[]

    name $datas[0];
    age $datas[1];

    echo name & " tiene " & age & " anos.";

    if age = 18:
        echo name + " es mayor de edad";
    elif age > 18:
        echo name + " es aldulto";
    else
        echo name + " es menor de edad";
    close;

    return 0;

close;

Number ret: main();
```

### KEYWORDS

- `if`
- `try`
- `elif`
- `else`
- `echo`
- `exit`
- `close`
- `raise`
- `while`
- `break*`
- `prompt`
- `typeof*`
- `return`
- `iferror*`
- `continue*`
- `function`
- `loadimport`

`*` -> unsure
