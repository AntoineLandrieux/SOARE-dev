# SOARE

## INTERPRETER

```txt
try
  loadimport "script.soare";
  Number ret: main(0;[]);
iferror
  writeln `"script.soare" is required.`;
close;

function Number main(Int argc; Array argv) 

  Array datas: ["", 0];

  String name datas[0];
  Int age datas[1];

  writeln name & " tiene " & age & " anos.";

  if age = 18:
    writeln name + " es mayor de edad";
  elsif age > 18:
    writeln name + " es aldulto";
  else:
    writeln name + " es menor de edad";
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
- `writeln`
  - `close`
  - `raise`
- `while`
- `prompt`
- `return`
  - `iferror`
- `function`
  - `loadimport`
