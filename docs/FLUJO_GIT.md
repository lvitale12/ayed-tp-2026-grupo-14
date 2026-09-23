# Sugerencias para Git y autoría

La cátedra evalúa el historial. Este documento no es un reglamento interno: reúne prácticas que podrían ayudar a organizarse y a mostrar el progreso real del grupo.

## Prácticas posibles

1. Usar una cuenta propia por integrante ayuda a identificar las contribuciones.
2. Mantener `master` estable puede simplificar la integración; probablemente no haga falta una rama `develop`.
3. Las ramas cortas por tarea, como `feat/normalizacion` o `feat/ventas`, permiten trabajar en paralelo.
4. Un pull request por tarea deja un espacio para explicar y revisar el cambio.
5. Una revisión de otra persona puede detectar incompatibilidades antes de integrar.
6. El **merge commit**, sin squash, conserva mejor la evolución de los commits.
7. Evitar compartir cuentas, cambiar autores o crear commits vacíos mantiene el historial representativo.
8. `Co-authored-by` puede reservarse para casos de trabajo en pareja real.

## Uso de herramientas de asistencia

Se pueden utilizar herramientas de IA o asistencia para acelerar diseño, implementación, depuración y pruebas, siempre que la política de la cátedra lo permita. No es necesario que cada carácter se escriba manualmente.

Para llegar preparados a la defensa, conviene que quien publique un cambio lo revise, compile, pruebe y entienda. También conviene conservar la autoría real y evitar divisiones artificiales destinadas únicamente a aparentar actividad.

## Qué constituye un buen commit

Un commit representa un hito que compila o una corrección comprobable. Ejemplos:

- `feat(normalizacion): leer registros históricos con fread`
- `feat(normalizacion): generar comandas diarias ordenadas`
- `feat(ventas): autenticar mozo contra mozos.dat`
- `fix(ventas): rechazar stock insuficiente sin modificar archivos`
- `feat(cierre): fusionar archivos diarios ordenados`
- `fix(cierre): admitir días sin archivo de comandas`
- `feat(resumen): realizar corte de control por mozo`
- `fix(resumen): emitir el último grupo y el total general`

Evitar mensajes como `cambios`, `avance`, `prueba` o divisiones artificiales para inflar el historial.

Como referencia, cada integrante probablemente produzca entre cinco y siete commits genuinos durante la semana: apertura y lectura, algoritmo principal, escritura, validaciones, pruebas y correcciones.

## Contenido sugerido para cada PR

- Requisito que resuelve.
- Cómo se compiló.
- Casos probados.
- Archivos que crea o modifica al ejecutarse.
- Pendientes o limitaciones conocidos.
