# SOARE

## INTERPRETER

```soare
try:
    loadimport "script.soare";
iferror:
    echo `"script.soare" is required.`;
    exit;
close;

function Number main(Int argc; Array argv) 

    Array datas: ["", 0];

    String name datas[0];
    Int age datas[1];

    echo name & " tiene " & age & " anos.";

    if age = 18:
        echo name + " es mayor de edad";
    elsif age > 18:
        echo name + " es aldulto";
    else:
        echo name + " es menor de edad";
    close;

    return 0;

close;

Number ret: main(0;[]);
```

### KEYWORDS

- `if`
- `nop`
- `try`
- `else`
- `exit`
- `write`
- `close`
- `raise`
- `while`
- `prompt`
- `return`
- `iferror`
- `continue*`
- `function`
- `loadimport`

`*` -> unsure
