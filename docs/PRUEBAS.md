# Plan de pruebas

## Oráculos del dataset oficial

Sin agregar ventas nuevas, `normalizacion.cpp` debe producir resultados compatibles con:

- 5 mozos únicos.
- 30 comandas en 6 fechas.
- 49 unidades vendidas.
- Comisión histórica total: 8770.

| Mozo | Comisión |
| --- | ---: |
| Sofia Ramirez | 1600 |
| Bruno Diaz | 1870 |
| Carla Lopez | 1810 |
| Diego Sosa | 1800 |
| Emma Ruiz | 1690 |

Stock esperado después de normalizar:

| Código | Stock |
| ---: | ---: |
| 101 | 51 |
| 105 | 69 |
| 110 | 26 |
| 112 | 34 |
| 120 | 43 |
| 125 | 26 |
| 130 | 21 |
| 140 | 36 |

Estos valores son oráculos de prueba; nunca deben quedar codificados en la solución.

## Casos límite obligatorios

- Archivo de entrada inexistente o vacío.
- ID de mozo inexistente.
- Contraseña incorrecta.
- Producto inexistente.
- Cantidad cero o negativa.
- Stock insuficiente.
- Archivo diario todavía inexistente.
- Uno o más días ausentes durante el cierre.
- Archivo semanal vacío.
- Un único mozo.
- Emisión correcta del último mozo en el corte de control.
- Una operación rechazada no modifica inventario, mozos ni comandas.

## Prueba final

Cada integrante debe clonar el repositorio en una carpeta nueva, copiar los `.dat` oficiales a la raíz, compilar los cuatro programas y ejecutar la cadena completa.
