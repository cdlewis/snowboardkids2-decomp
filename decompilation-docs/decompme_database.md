# decomp.me Database Tables

This document describes the useful tables in a PostgreSQL import of a decomp.me database dump. It is intended as a query guide for analysis tools and LLM agents.

The imported data is centered on decomp.me scratches: source code submissions, compiler settings, target assembly/object data, metadata, scores, and relationships between related scratches.

## Table Overview

The dump contains three main tables:

```text
public.coreapp_scratch   scratch metadata, source code, context, compiler settings, scores
public.coreapp_assembly  compiled target object records referenced by scratches
public.coreapp_asm       original assembly text referenced by assembly records
```

Observed row counts in one import:

```text
public.coreapp_asm       156,572 rows
public.coreapp_assembly  146,611 rows
public.coreapp_scratch   175,474 rows
```

The dump may not include every table from the production application. In particular, user/profile and preset tables may be absent, so `owner_id` and `preset_id` should be treated as opaque identifiers unless matching tables are available in the local import.

## Query Basics

Use PostgreSQL syntax and fully qualified table names under the `public` schema.

Example one-off query:

```sh
psql -d DATABASE_NAME -c "SELECT count(*) FROM public.coreapp_scratch;"
```

When exploring, avoid broad `SELECT *` queries. Some columns are large:

```text
public.coreapp_scratch.source_code
public.coreapp_scratch.context
public.coreapp_asm.data
public.coreapp_assembly.elf_object
```

Prefer selecting only needed columns and using `LIMIT`.

## `public.coreapp_scratch`

Main scratch table. One row is one decomp.me scratch.

Important columns:

```text
slug                 scratch identifier; used in decomp.me scratch URLs
creation_time        scratch creation timestamp
last_updated         last update timestamp
compiler             compiler identifier
compiler_flags       compiler flags used by the scratch
source_code          submitted source code
context              context/header/source context used for compilation
target_assembly_id   references coreapp_assembly.hash
diff_label           symbol/function label
platform             target platform, e.g. gba, gc_wii
score                current diff score
max_score            max possible score
name                 scratch name
description          scratch description
diff_flags           JSONB diff settings
libraries            JSONB library metadata, nullable
parent_id            parent scratch slug, nullable
family_id            related scratch family slug, nullable
owner_id             original application user id, nullable
preset_id            original preset id, nullable
claim_token          nullable
```

Primary key:

```text
coreapp_scratch.slug
```

Useful indexes typically include:

```text
platform
owner_id
target_assembly_id
parent_id
family_id
upper(name::text)
upper(diff_label::text)
```

Self-references:

```text
parent_id -> coreapp_scratch.slug
family_id -> coreapp_scratch.slug
```

## `public.coreapp_assembly`

Compiled target object table. Scratches point here through `coreapp_scratch.target_assembly_id`.

Columns:

```text
hash            assembly/object hash, primary key
time            timestamp
arch            architecture identifier
elf_object      compiled ELF object bytes as bytea
source_asm_id   references coreapp_asm.hash, nullable
```

Primary key:

```text
coreapp_assembly.hash
```

Join from scratches:

```sql
SELECT s.slug, s.name, s.platform, a.arch, a.hash
FROM public.coreapp_scratch AS s
JOIN public.coreapp_assembly AS a
    ON a.hash = s.target_assembly_id
LIMIT 20;
```

## `public.coreapp_asm`

Original assembly text table.

Columns:

```text
hash   assembly text hash, primary key
data   assembly text
```

Primary key:

```text
coreapp_asm.hash
```

Join from assembly records:

```sql
SELECT a.hash AS assembly_hash, a.arch, asm.data
FROM public.coreapp_assembly AS a
LEFT JOIN public.coreapp_asm AS asm
    ON asm.hash = a.source_asm_id
WHERE a.hash = 'PUT_ASSEMBLY_HASH_HERE';
```

## Relationship Map

Use this join path to get scratch metadata, source code, compiler settings, compiled object metadata, and source assembly text:

```sql
SELECT
    s.slug,
    s.creation_time,
    s.last_updated,
    s.compiler,
    s.compiler_flags,
    s.platform,
    s.name,
    s.diff_label,
    s.score,
    s.max_score,
    s.source_code,
    s.context,
    s.diff_flags,
    s.libraries,
    a.arch,
    a.hash AS assembly_hash,
    asm.data AS source_asm
FROM public.coreapp_scratch AS s
JOIN public.coreapp_assembly AS a
    ON a.hash = s.target_assembly_id
LEFT JOIN public.coreapp_asm AS asm
    ON asm.hash = a.source_asm_id
WHERE s.slug = 'PUT_SLUG_HERE';
```

Relationship summary:

```text
coreapp_scratch.target_assembly_id -> coreapp_assembly.hash
coreapp_assembly.source_asm_id     -> coreapp_asm.hash
coreapp_scratch.parent_id          -> coreapp_scratch.slug
coreapp_scratch.family_id          -> coreapp_scratch.slug
```

## Starter Queries

Most recent scratches:

```sql
SELECT slug, creation_time, compiler, platform, score, max_score, name
FROM public.coreapp_scratch
ORDER BY creation_time DESC
LIMIT 20;
```

Find scratches by platform:

```sql
SELECT slug, creation_time, compiler, score, max_score, name
FROM public.coreapp_scratch
WHERE platform = 'gc_wii'
ORDER BY creation_time DESC
LIMIT 50;
```

Find scratches by compiler:

```sql
SELECT slug, creation_time, platform, score, max_score, name
FROM public.coreapp_scratch
WHERE compiler = 'mwcc_233_163n'
ORDER BY creation_time DESC
LIMIT 50;
```

Search for a function, symbol, or scratch name:

```sql
SELECT slug, creation_time, compiler, platform, score, max_score, name, diff_label
FROM public.coreapp_scratch
WHERE diff_label ILIKE '%FUNCTION_OR_SYMBOL_NAME%'
   OR name ILIKE '%FUNCTION_OR_SYMBOL_NAME%'
ORDER BY creation_time DESC
LIMIT 100;
```

Search submitted source code:

```sql
SELECT slug, creation_time, compiler, platform, name, diff_label
FROM public.coreapp_scratch
WHERE source_code ILIKE '%some_identifier%'
ORDER BY creation_time DESC
LIMIT 50;
```

Search assembly text:

```sql
SELECT s.slug, s.creation_time, s.compiler, s.platform, s.name, s.diff_label
FROM public.coreapp_scratch AS s
JOIN public.coreapp_assembly AS a
    ON a.hash = s.target_assembly_id
JOIN public.coreapp_asm AS asm
    ON asm.hash = a.source_asm_id
WHERE asm.data ILIKE '%some_instruction_or_label%'
ORDER BY s.creation_time DESC
LIMIT 50;
```

Inspect JSON settings:

```sql
SELECT slug, diff_flags, libraries
FROM public.coreapp_scratch
WHERE libraries IS NOT NULL
LIMIT 20;
```

Aggregate by platform and compiler:

```sql
SELECT platform, compiler, count(*) AS scratches
FROM public.coreapp_scratch
GROUP BY platform, compiler
ORDER BY scratches DESC;
```

Find high-quality or solved scratches:

```sql
SELECT slug, creation_time, compiler, platform, score, max_score, name, diff_label
FROM public.coreapp_scratch
WHERE score = 0
   OR score = max_score
ORDER BY creation_time DESC
LIMIT 100;
```

Find scratch families:

```sql
SELECT family_id, count(*) AS scratches
FROM public.coreapp_scratch
WHERE family_id IS NOT NULL
GROUP BY family_id
ORDER BY scratches DESC
LIMIT 50;
```

Follow parent/child scratch relationships:

```sql
SELECT
    child.slug AS child_slug,
    child.name AS child_name,
    parent.slug AS parent_slug,
    parent.name AS parent_name
FROM public.coreapp_scratch AS child
JOIN public.coreapp_scratch AS parent
    ON parent.slug = child.parent_id
LIMIT 50;
```

Export a scratch's source and assembly from `psql`:

```sql
\pset tuples_only on
\pset format unaligned
\o source.c
SELECT source_code FROM public.coreapp_scratch WHERE slug = 'PUT_SLUG_HERE';
\o source.s
SELECT asm.data
FROM public.coreapp_scratch AS s
JOIN public.coreapp_assembly AS a ON a.hash = s.target_assembly_id
LEFT JOIN public.coreapp_asm AS asm ON asm.hash = a.source_asm_id
WHERE s.slug = 'PUT_SLUG_HERE';
\o
```

## LLM Query Guidance

- Start from `public.coreapp_scratch`; it is the main user-facing entity.
- Join to `public.coreapp_assembly` only when architecture or binary object data is needed.
- Join to `public.coreapp_asm` only when original assembly text is needed.
- Do not select `elf_object` in exploratory queries unless binary object bytes are specifically required.
- Use `LIMIT` while exploring and narrow by `platform`, `compiler`, `name`, `diff_label`, or `slug`.
- For case-insensitive symbol/name search, use `ILIKE` or `upper(name::text)` / `upper(diff_label::text)`.
- Treat absent app tables as normal for partial dumps; `owner_id` and `preset_id` may not be resolvable.
