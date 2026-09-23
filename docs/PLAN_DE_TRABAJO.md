# Plan de trabajo — Grupo 14

Moodle muestra vencimiento el **30/09/2026 a las 00:00**. Para no depender de una interpretación ambigua, el límite interno del grupo será el **29/09 a las 12:00** y el envío se hará durante ese día.

## Dificultad y criterio de reparto

El problema no exige C++ avanzado. La dificultad está en mantener compatibles los archivos binarios y conseguir que funcionen los cuatro programas en cadena.

| Programa | Dificultad aproximada | Estado |
| --- | ---: | --- |
| `normalizacion.cpp` | 8/10 | sin asignar |
| `ventas.cpp` | 7/10 | sin asignar |
| `cierre.cpp` | 6/10 | sin asignar |
| `resumen.cpp` | 4/10 | sin asignar |

Todavía no hay programas asignados. El reparto podría definirse cuando todos indiquen experiencia, disponibilidad y preferencia. Para llegar a tiempo, una opción es priorizar las tareas críticas y distribuirse según lo que más ayude al avance general.

`normalizacion.cpp` y `ventas.cpp` tienen mayor carga. Para equilibrar, quienes tomen `cierre.cpp` o `resumen.cpp` podrían ayudar además con pruebas, integración o una tarea acotada de los programas más grandes.

Una distribución posible es tener una persona de referencia por programa y hacer algunas revisiones cruzadas, procurando que todos conozcan la cadena completa para la defensa.

## Calendario orientativo

Las siguientes fechas son una propuesta para visualizar el tiempo disponible. El grupo puede adaptarlas según su disponibilidad.

### 22/09 — Preparación

- Incorporar a todos al repositorio.
- Leer el enunciado completo.
- Congelar el contrato común.
- Asignar programas y abrir una rama por tarea.
- Si resulta útil, cada integrante puede crear un esqueleto compilable y registrar un primer avance propio.

### 23–25/09 — Desarrollo individual

- Implementar cada programa por hitos pequeños y comprobables.
- Probar con archivos mínimos antes de integrar.
- Abrir PR tempranos, aunque todavía estén en borrador.
- Hacer una revisión cruzada en parejas.

### 26/09 — Primera integración

Ejecutar por primera vez la cadena:

```text
copias limpias
→ normalizacion
→ ventas
→ cierre
→ resumen
```

Corregir incompatibilidades sin cambiar unilateralmente structs, nombres de archivo o criterios de orden.

### 27–28/09 — Casos límite y defensa

- Cubrir todos los casos de `docs/PRUEBAS.md`.
- Probar desde un clon limpio.
- Completar README y decisiones.
- Puede ser útil que cada integrante practique la explicación de su parte y conozca al menos otra.

### 29/09 — Cierre

- Congelar funcionalidad al mediodía.
- Compilar y ejecutar desde cero.
- Revisar autores, commits y archivos versionados.
- Hacer público el repositorio o garantizar acceso docente.
- Enviar el enlace por correo antes de las 23:59, idealmente con varias horas de margen.

## Regla de alcance

Primero se busca una solución simple y defendible con la teoría de la cátedra. No agregar STL avanzada, orientación a objetos, bases de datos, interfaces gráficas ni librerías externas.
