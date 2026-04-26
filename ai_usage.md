# AI Usage Documentation — Phase 1 Filter Functions

## Tool Used
Claude (Anthropic) — claude.ai chat interface

---

## Functions Generated with AI Assistance

### 1. `parse_condition`

**Prompt given to AI:**
> I have a C struct called `Record` with fields: `id` (int), `inspector_name` (char[50]),
> `latitude` (float), `longitude` (float), `issue_category` (char[50]),
> `security_level` (int), `timestamp` (size_t), `description` (char[256]).
> Please generate a function:
> `int parse_condition(const char *input, char *field, char *op, char *value);`
> that splits a string of the form "field:operator:value" into its three parts.
> Return 1 on success, 0 on failure.

**What was generated:**
The AI produced a function using `strchr()` to find the two colons,
null-terminating at each split point, and copying into `field`, `op`, `value`
using `strcpy()`. It worked on a local copy of the input with `strncpy()`.

**What I changed and why:**
- Added a `buf` local copy with explicit null-termination to avoid modifying
  the caller's string. The AI version used `strdup()` which requires `free()`;
  I replaced it with a stack buffer to keep memory management simple.
- Verified that the function returns 0 correctly when either colon is missing.

**What I learned:**
The AI correctly identified `strchr()` as the right tool and produced clean code.
The only issue was memory management — using `strdup()` without a matching `free()`
would cause a memory leak. This reinforced the importance of reviewing generated code
for resource management, not just logic.

---

### 2. `match_condition`

**Prompt given to AI:**
> Using the same `Record` struct above, generate a function:
> `int match_condition(Record *r, const char *field, const char *op, const char *value);`
> that returns 1 if record r satisfies the condition field:op:value and 0 otherwise.
> Supported fields: severity, category, inspector, timestamp.
> Supported operators: ==, !=, <, <=, >, >=.

**What was generated:**
The AI produced a chain of `strcmp(field, ...)` checks, with inner blocks for
each operator. It used `atoi()` for integer fields and `strcmp()` for string fields.

**What I changed and why:**
- The AI used `atoi()` for the `timestamp` field, which on a 64-bit system
  would truncate large Unix timestamps (stored as `size_t`). I replaced it with
  `atoll()` cast to `size_t` to handle full 64-bit values correctly.
- The AI did not handle the case where `field` is unrecognised — it would just
  return 0 silently. I kept the same behaviour but added a comment explaining why.
- String operators like `<` and `>` were included in the AI output for string fields.
  I removed those since lexicographic ordering on inspector names or categories is
  not meaningful for our use case, and the spec does not require it.

**What I learned:**
The AI's logic was structurally correct but missed the type-width issue with
`timestamp`. It is important to match the C type (`size_t`) to the parsing function
(`atoll`), which requires understanding both the record definition and platform
integer widths. The AI cannot know these without being told explicitly.

---

## Summary

Both functions were reviewed line by line. The core logic (splitting on colons,
comparing fields with the right operators) was correct as generated. The changes
I made were targeted fixes for:
1. Memory management (`strdup` → stack buffer)
2. Integer width (`atoi` → `atoll` for `timestamp`)
3. Removing unsupported operators for string fields

The filter loop itself (opening `reports.dat` with `open()`, calling
`parse_condition()` and `match_condition()` in a `read()` loop, printing
matching records) was written entirely by me without AI assistance.
