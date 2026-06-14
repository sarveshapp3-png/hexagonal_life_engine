# Synthetic Lab Design

This design system is built for a high-tech biological simulation, blending the
precision of a scientific laboratory with the vibrant, emergent energy of
artificial life.

## Brand Direction

- Clinical.
- Immersive.
- Exploratory.
- Dense enough for a power user, but still readable at a glance.
- The source style leans toward a technical glassmorphism look rather than a
  playful game UI.
- The official Life Engine title mark is treated as reference branding, not as
  the final UI system.

## Visual Language

- Deep navy base surfaces anchored by `#081425` and `#0f172a`.
- Neon green, cyan, pink, and yellow accents with a restrained glow.
- Thin 1px borders and subtle blur rather than heavy shadows.
- Small radii, compact spacing, and high information density.
- Technical dashboards, not playful game panels.
- Deep layering for simulation, panels, and overlays so the canvas still feels
  physically separate from the controls.

## Typography

- Headings: `Hanken Grotesk`.
- Body: `Inter`.
- Labels and stats: `JetBrains Mono`.
- Use tabular figures for live numeric readouts so counters do not jump.

## Layout Rules

- Keep the simulation canvas dominant, with the viewer taking roughly the top
  or central majority of the viewport.
- Put controls in clean card stacks and drawers.
- Prefer rectangular UI panels.
- Reserve hex geometry for the simulation world itself.
- Use a fluid dashboard layout on desktop and collapsible bottom sheets on
  mobile.

## Components

- Simulation controls should favor dense readouts and obvious state changes.
- Stat cards should surface FPS, cell count, and energy-style summaries.
- Genome editor tools should feel technical and data-first.
- Navigation should stay thin and unobtrusive so the simulation remains the
  focal point.

## Related Notes

- [[Project Brief The Life Engine Hex]]
- [[Architecture/Hex Engine Architecture]]
- [[Branding/Life Engine Logo]]
