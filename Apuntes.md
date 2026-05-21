# Codexion — Conceptos clave

---

## 1. Concurrencia básica

### Hilo (thread)
Unidad de ejecución dentro de un programa. Varios hilos corren "a la vez" compartiendo la misma memoria del proceso.

**Analogía:** varios cocineros trabajando en la misma cocina al mismo tiempo, compartiendo los mismos utensilios.

En Codexion: **cada persona que programa = un hilo.**

### Mutex (mutual exclusion)
Un "cartel de ocupado" que garantiza que solo un hilo a la vez puede acceder a un recurso compartido.

- `lock` → cojo el recurso, pongo el cartel en "ocupado"
- `unlock` → libero el recurso, pongo el cartel en "libre"

Entre el `lock` y el `unlock` solo UN hilo puede estar. Los demás esperan fuera.

En Codexion: **cada dongle está protegido por un mutex.**

### Condición de carrera (race condition)
Cuando dos hilos tocan el mismo dato a la vez sin protección → comportamiento impredecible y bugs muy difíciles de reproducir.

**Ejemplo:** dos personas cogen el mismo dongle a la vez sin mutex → el estado del dongle se corrompe.

---

## 2. El problema de los filósofos comensales

Origen del problema que Codexion simula. 5 filósofos en mesa circular, cada uno necesita **dos tenedores** para comer, hay exactamente 5 tenedores (uno entre cada par).

| Filósofos clásico | Codexion |
|---|---|
| Filósofo | Persona que programa |
| Pensar | Depurar / refactorizar |
| Comer | Compilar |
| Tenedor | Dongle USB |
| 2 tenedores para comer | 2 dongles para compilar |

---

## 3. Deadlock (interbloqueo)

Todos los hilos esperan algo que tiene otro. **Nadie avanza. Para siempre.**

### Ejemplo visual con 3 personas

```
        Persona 1
       /          \
  Dongle 3      Dongle 1
     /                \
Persona 3 — Dongle 2 — Persona 2
```

```
Momento 0:
  Persona 1 coge Dongle 1 ✋
  Persona 2 coge Dongle 2 ✋
  Persona 3 coge Dongle 3 ✋

Momento 1:
  Persona 1 quiere Dongle 3... lo tiene Persona 3 😐
  Persona 2 quiere Dongle 1... lo tiene Persona 1 😐
  Persona 3 quiere Dongle 2... lo tiene Persona 2 😐

Momento 2:
  Todos esperando. Nadie suelta nada. Nadie avanza. 💀
```

### Las 4 condiciones de Coffman

Para que ocurra un deadlock deben cumplirse **las cuatro** a la vez. Romper cualquiera lo evita.

| Condición | Qué significa | En el ejemplo |
|---|---|---|
| **Exclusión mutua** | Un recurso solo lo usa uno a la vez | Cada dongle solo lo tiene una persona |
| **Retención y espera** | Tengo algo y espero más | Cada uno retiene su dongle y espera el otro |
| **Sin apropiación** | No puedo quitar el recurso a la fuerza | Nadie suelta lo que tiene |
| **Espera circular** | A espera a B, B espera a C, C espera a A | 1→3→2→1 |

---

## 4. Starvation (inanición / agotamiento)

Diferente al deadlock. Nadie está bloqueado para siempre en teoría, pero en la práctica una persona **nunca tiene suerte** — siempre hay alguien que se adelanta.

En Codexion: si una persona no compila dentro de `time_to_burnout` milisegundos → **burned out** → la simulación se detiene.

---

## 5. Scheduling — FIFO y EDF

Cuando varias personas quieren el mismo dongle a la vez, el **scheduler** decide a quién se lo damos.

### FIFO — First In, First Out
El dongle se da al que llegó primero a la cola. Simple, pero **no previene starvation** — si alguien está a punto de agotarse pero llegó tarde, mala suerte.

### EDF — Earliest Deadline First
El dongle se da al que tiene el **deadline más próximo**.

```
deadline = momento_ultimo_inicio_compilacion + time_to_burnout
```

**Ejemplo:**
```
Tiempo actual: t = 800ms
time_to_burnout = 500ms

Persona 3 → última compilación en t=400ms → deadline = 900ms → le quedan 100ms ⚠️
Persona 1 → última compilación en t=700ms → deadline = 1200ms → le quedan 400ms
Persona 2 → última compilación en t=750ms → deadline = 1250ms → le quedan 450ms

Decisión EDF: dongle para Persona 3
```

EDF compara los deadlines **absolutos** y elige el más pequeño — el que vence antes.

Con parámetros viables, EDF **garantiza que nadie se agota**.

| | FIFO | EDF |
|---|---|---|
| Criterio | Orden de llegada | Urgencia (deadline) |
| ¿Previene starvation? | No garantizado | Sí, con parámetros viables |
| Complejidad | Simple | Más complejo |

---

## 6. Heap (montículo)

Estructura de datos que mantiene siempre el elemento más urgente accesible de forma eficiente. Necesario para implementar las colas de FIFO y EDF.

### La clave: un árbol dentro de un array

No hay punteros. Las relaciones padre/hijo son **matemáticas**:

```
hijo izquierdo de índice i  →  2*i + 1
hijo derecho de índice i    →  2*i + 2
padre de índice i           →  (i - 1) / 2
```

**Ejemplo visual:**
```
           [0] P3 (deadline 900)
          /                     \
   [1] P1 (1200)           [2] P2 (1250)
   /          \
[3] P5 (1400) [4] P4 (1600)

Array: [ P3 | P1 | P2 | P5 | P4 ]
índice:  [0]  [1]  [2]  [3]  [4]
```

### En memoria hay dos cosas separadas

```
HEAP (el organizador):
[ *P3 | *P1 | *P2 | *P5 | *P4 ]  ← array de punteros
    ↓      ↓      ↓      ↓      ↓

PERSONAS (independientes en memoria):
P1{id:1, deadline:1200, ...}
P2{id:2, deadline:1250, ...}
P3{id:3, deadline:900,  ...}
```

El heap reorganiza sus punteros, las personas no se mueven de sitio en memoria.

### En código

```c
typedef struct s_heap
{
    t_coder **data;   // array de punteros a personas
    int     size;     // cuántas personas hay ahora mismo
    int     capacity; // tamaño máximo del array
}   t_heap;
```

Dos mallocs necesarios:
```c
heap->data = malloc(sizeof(t_coder *) * capacity); // el array del heap
coders[i]  = malloc(sizeof(t_coder));              // cada persona
```

### Heap vs lista enlazada

| | Lista enlazada ordenada | Heap |
|---|---|---|
| Insertar | O(n) — recorre toda la lista | O(log n) — solo sube por el árbol |
| Extraer mínimo | O(1) — cabeza de la lista | O(log n) — reordena el árbol |
| Con 100 personas | hasta 100 comparaciones | máximo 7 comparaciones |

---

## 7. Dongle cooldown

Después de ser liberado, un dongle **no puede cogerse hasta que pasen X milisegundos**.

```
t=500ms → Persona 1 suelta Dongle 2
           cooldown = 100ms
           Dongle 2 no disponible hasta t=600ms

t=550ms → Persona 3 quiere Dongle 2
           Tiene que esperar aunque nadie lo tenga
```

Complica el scheduling porque un dongle puede estar **libre pero no disponible**. También agrava el riesgo de deadlock — una persona puede retener un dongle mientras el otro está en cooldown.

---

## 8. Hilo monitor

Hilo extra cuyo único trabajo es vigilar a todas las personas y detectar burnout.

**¿Por qué no lo detecta cada persona sola?**
Porque una persona bloqueada esperando un dongle no puede ejecutar código para darse cuenta de que se está agotando. Necesita un vigilante externo.

El monitor hace en bucle:
```
cada pocos milisegundos:
    tiempo_actual = ahora
    para cada persona:
        si (tiempo_actual - ultimo_inicio_compilacion) > time_to_burnout:
            imprime "X burned out"
            detiene la simulacion
```

**Requisito del subject:** el mensaje de burnout debe aparecer en **máximo 10ms** desde el momento real del agotamiento.

---

## 9. Log serializado

Un mutex protege el `printf` para que los mensajes de distintos hilos nunca se mezclen en la misma línea.

**Formato de cada mensaje:**
```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

---

## 10. Cuadro resumen global

| Qué | Cómo |
|---|---|
| Cada persona | Un hilo `pthread` |
| Cada dongle | Una struct protegida con mutex |
| Acceso justo al dongle | Heap + scheduler FIFO o EDF |
| Detectar burnout | Hilo monitor separado |
| Cooldown | El dongle guarda su timestamp de liberación |
| Logs sin mezclar | Mutex que protege el printf |
| Prevenir deadlock | Romper alguna condición de Coffman |
| Prevenir starvation | Scheduler EDF con parámetros viables |