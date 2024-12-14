
# ![LOGO](resources/icon/icon.svg) SOARE

---

## INTERPRETER

```txt
try
  loadimport "script.soare";
iferror
  writeln `Error: can't import all files.`;
  quit;
close;

? Main function
? Take 2 arguments (name and age)
function Int main(String first_name; String last_name; Int age)

  def
    String full_name +
    String message +
    Int estMajeur;

  full_name = first_name + " " + last_name
  message = full_name + " a " + age;
  estMajeur = 1

  writeln message;

  if age equ 18 do
    writeln full_name + " est tout juste majeur";
  elseif age > 18 do
    writeln full_name + " est un adulte";
  else
    writeln full_name + " est un enfant";
    estMajeur = 0
  close;

  return estMajeur;

close;

main!("Antoine"; "Landrieux"; 14)

```
