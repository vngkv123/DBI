# pintool
**Easy to use Instrumentation**
- Use dynamic instrumentation
- Provides rich APIs to write in C/C++ your own instrumentation tools.


# How to use?
**Launch**
- `pin -t pintool -- application`
- `pin` is instrumentation engine
- `pintool` is Intrumentation tool.( write your own, or use one provided in the kit )

**attach to and instrument an application**
- `pin -t pintool -pid [pid]`

# APIs
**Basic APIs**
- Architecture independent.
- Control-flow changes
- MEmory accesses

**Call-based APIs**
- Instrumentation routines
- Analysis routines

