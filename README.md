
# ![LOGO](resources/icon/icon.svg) SOARE

---

## INTERPRETER

```txt
try
  loadimport "script.soare";
  if @main<"Antoine";14> do
    raise "ProgramError";
  close;
iferror
  writeln `Error: Module or program error`;
close;

? Main function
? Take 2 arguments (name and age)
function Int main <String name; Int age> 

  writeln name, " a ", age, " ans.";

  if age equ 18 do
    writeln name, " est tout juste majeur";
  else age > 18 do
    writeln name, " est un adulte";
  else do
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
