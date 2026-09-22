# Plan de trabajo — Grupo 14

Moodle muestra vencimiento el **30/09/2026 a las 00:00**. Para no depender de una interpretación ambigua, el límite interno del grupo será el **29/09 a las 12:00** y el envío se hará durante ese día.

## Dificultad y reparto propuesto

El problema no exige C++ avanzado. La dificultad está en mantener compatibles los archivos binarios y conseguir que funcionen los cuatro programas en cadena.

| Programa | Dificultad aproximada | Responsable propuesto |
| --- | ---: | --- |
| `normalizacion.cpp` | 8/10 | integrante con mayor soltura, con apoyo de quien haga cierre |
| `ventas.cpp` | 7/10 | segundo integrante con mayor soltura |
| `cierre.cpp` | 6/10 | un integrante, más apoyo acotado a normalización |
| `resumen.cpp` | 4/10 | Lorenzo |

Lorenzo también coordina el repositorio, integración, README y entrega. El reparto definitivo se cierra cuando todos indiquen experiencia, disponibilidad y preferencia.

Cada integrante será responsable principal de un programa, pero deberá revisar al menos un PR ajeno y poder explicar la cadena completa en la defensa.

## Calendario

### 22/09 — Preparación

- Incorporar a todos al repositorio.
- Leer el enunciado completo.
- Congelar el contrato común.
- Asignar programas y abrir una rama por tarea.
- Cada integrante crea un esqueleto compilable y realiza su primer commit propio.

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
- Cada integrante explica su parte y una parte ajena.

### 29/09 — Cierre

- Congelar funcionalidad al mediodía.
- Compilar y ejecutar desde cero.
- Revisar autores, commits y archivos versionados.
- Hacer público el repositorio o garantizar acceso docente.
- Enviar el enlace por correo antes de las 23:59, idealmente con varias horas de margen.

## Regla de alcance

Primero se busca una solución simple y defendible con la teoría de la cátedra. No agregar STL avanzada, orientación a objetos, bases de datos, interfaces gráficas ni librerías externas.
