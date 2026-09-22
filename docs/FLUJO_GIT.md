# Flujo de Git y autoría

La cátedra evalúa el historial. El objetivo es mostrar trabajo real, no fabricar actividad.

## Reglas

1. Cada integrante trabaja desde su propia cuenta y computadora.
2. `master` se mantiene estable; no hace falta una rama `develop`.
3. Cada tarea se hace en una rama corta: `feat/normalizacion`, `feat/ventas`, `feat/cierre` o `feat/resumen`.
4. Cada autor abre su propio pull request.
5. Otra persona revisa el PR; el autor original realiza las correcciones.
6. Se integra con **merge commit**, sin squash, para conservar el progreso.
7. No se comparten cuentas, no se cambian autores y no se crean commits vacíos.
8. `Co-authored-by` se usa únicamente cuando hubo trabajo en pareja real.

## Uso de herramientas de asistencia

Se pueden utilizar herramientas de IA o asistencia para acelerar diseño, implementación, depuración y pruebas, siempre que la política de la cátedra lo permita. No es necesario que cada carácter se escriba manualmente.

La responsabilidad del commit sigue siendo de quien lo publica: debe revisar el cambio, compilarlo, probarlo y poder explicar las decisiones y el algoritmo durante la defensa. No se atribuye código a otra cuenta ni se divide artificialmente para aparentar trabajo.

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

## Contenido mínimo de cada PR

- Requisito que resuelve.
- Cómo se compiló.
- Casos probados.
- Archivos que crea o modifica al ejecutarse.
- Pendientes o limitaciones conocidos.
