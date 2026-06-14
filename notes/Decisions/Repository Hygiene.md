# Repository Hygiene

This note records the practical source-control rules for the workspace.

## Ignored Files

- Build outputs stay out of version control.
- Editor caches and machine-specific settings stay out of version control.
- The `stitch_hexagonal_life_engine/` folder stays local reference material.

## License Choice

- The current project license should be `GPL-3.0`.
- That matches the license family used by the LifeEngine reference repository.
- It also keeps shared modifications and future contributions under the same
  copyleft terms.

## Notes For Collaboration

- Source, notes, and assets that belong to the project should be committed.
- Generated binaries, temporary files, and local setup files should not be
  committed.
- Reference art from external sources should be reviewed separately before any
  public release.

## Related Notes

- [[Foundation Decisions]]
- [[Lua Runtime Strategy]]
- [[Checklist/Laptop And Project Setup]]
