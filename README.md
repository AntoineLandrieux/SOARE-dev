# SOARE

## INTERPRETER

```txt
try
  loadimport "script.soare";
  if @main<"Antoine";14> do
    raise "ProgramError";
  close;
iferror
  writeln `"script.soare" is required.`;
close;

function Int main <String name; Int age> 

  writeln name, " a ", age, " ans.";

  if age = 18:
    writeln name, " est tout juste majeur";
  elsif age > 18:
    writeln name, " est un adulte";
  else:
    writeln name, " est un enfant";
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
