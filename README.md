# SOARE

## INTERPRETER

```txt
try
    loadimport "script.soare";
    Number ret: main(0;[]);
iferror
    echo `"script.soare" is required.`;
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
```

### ADVENCEMENT

```txt
try
  nop;
iferror
  raise "Error";
close;
```

### KEYWORDS

- `if`
  - `nop`
  - `try`
- `else`
- `write`
- `close`
  - `raise`
- `while`
- `prompt`
- `return`
  - `iferror`
- `function`
  - `loadimport`
