---
name: Synthetic Lab
colors:
  surface: '#081425'
  surface-dim: '#081425'
  surface-bright: '#2f3a4c'
  surface-container-lowest: '#040e1f'
  surface-container-low: '#111c2d'
  surface-container: '#1e293b'
  surface-container-high: '#1f2a3c'
  surface-container-highest: '#2a3548'
  on-surface: '#d8e3fb'
  on-surface-variant: '#bccbb9'
  inverse-surface: '#d8e3fb'
  inverse-on-surface: '#263143'
  outline: '#869585'
  outline-variant: '#3d4a3d'
  surface-tint: '#4ae176'
  primary: '#4be277'
  on-primary: '#003915'
  primary-container: '#22c55e'
  on-primary-container: '#004b1e'
  inverse-primary: '#006e2f'
  secondary: '#ffafd3'
  on-secondary: '#620040'
  secondary-container: '#85145a'
  on-secondary-container: '#ff93c8'
  tertiary: '#f0c300'
  on-tertiary: '#3c2f00'
  tertiary-container: '#cfa800'
  on-tertiary-container: '#4f3e00'
  error: '#ffb4ab'
  on-error: '#690005'
  error-container: '#93000a'
  on-error-container: '#ffdad6'
  primary-fixed: '#6bff8f'
  primary-fixed-dim: '#4ae176'
  on-primary-fixed: '#002109'
  on-primary-fixed-variant: '#005321'
  secondary-fixed: '#ffd8e7'
  secondary-fixed-dim: '#ffafd3'
  on-secondary-fixed: '#3d0026'
  on-secondary-fixed-variant: '#85145a'
  tertiary-fixed: '#ffe083'
  tertiary-fixed-dim: '#eec200'
  on-tertiary-fixed: '#231b00'
  on-tertiary-fixed-variant: '#574500'
  background: '#081425'
  on-background: '#d8e3fb'
  surface-variant: '#2a3548'
  surface-bg: '#0f172a'
  surface-overlay: '#334155'
  neon-green: '#22c55e'
  neon-pink: '#f472b6'
  neon-yellow: '#facc15'
  neon-cyan: '#22d3ee'
  data-red: '#ef4444'
typography:
  headline-lg:
    fontFamily: Hanken Grotesk
    fontSize: 32px
    fontWeight: '700'
    lineHeight: 40px
    letterSpacing: -0.02em
  headline-md:
    fontFamily: Hanken Grotesk
    fontSize: 24px
    fontWeight: '600'
    lineHeight: 32px
  body-lg:
    fontFamily: Inter
    fontSize: 16px
    fontWeight: '400'
    lineHeight: 24px
  body-md:
    fontFamily: Inter
    fontSize: 14px
    fontWeight: '400'
    lineHeight: 20px
  label-md:
    fontFamily: JetBrains Mono
    fontSize: 12px
    fontWeight: '500'
    lineHeight: 16px
    letterSpacing: 0.05em
  label-sm:
    fontFamily: JetBrains Mono
    fontSize: 10px
    fontWeight: '500'
    lineHeight: 14px
  headline-lg-mobile:
    fontFamily: Hanken Grotesk
    fontSize: 28px
    fontWeight: '700'
    lineHeight: 34px
rounded:
  sm: 0.125rem
  DEFAULT: 0.25rem
  md: 0.375rem
  lg: 0.5rem
  xl: 0.75rem
  full: 9999px
spacing:
  base: 4px
  gutter-xs: 8px
  gutter-md: 16px
  margin-mobile: 16px
  margin-desktop: 32px
  panel-gap: 12px
---

## Brand & Style

This design system is built for a high-tech biological simulation, blending the precision of a scientific laboratory with the vibrant, emergent energy of artificial life. The brand personality is **clinical, immersive, and exploratory**. It targets researchers, simulation enthusiasts, and "God-game" players who value data density and aesthetic clarity.

The visual style is a hybrid of **Modern Corporate** and **Technical Glassmorphism**. It utilizes a deep, nocturnal foundation to make neon biological elements pop with "self-illuminated" intensity. The aesthetic suggests a high-end software console used in a near-future genetics lab—clean, structured, and unapologetically technical.

## Colors

The palette is anchored by a "Nocturnal Slate" spectrum to provide maximum contrast for simulation data. 

- **Primary (Neon Green):** Represents vitality, photosynthesis, and active energy.
- **Secondary (Neon Pink):** Used for predatory cells, mutations, or critical warnings.
- **Tertiary (Neon Yellow):** Used for metabolic waste, catalysts, or secondary energy sources.
- **Neutral:** A deep slate blue that provides a "lab-at-night" atmosphere, reducing eye strain during long simulation sessions.

The color mode is exclusively **Dark**. Backgrounds use the deepest slate (#0f172a), while interactive containers use the slightly lighter slate (#1e293b). UI elements should feel like they are glowing against the dark void of the simulation environment.

## Typography

The typographic system prioritizes legibility and a technical "readout" feel. 

- **Headlines:** Use **Hanken Grotesk** for a sharp, contemporary, and professional appearance.
- **Body:** **Inter** provides a neutral, highly readable foundation for long-form data or descriptions.
- **Labels & Stats:** **JetBrains Mono** is utilized for all numerical readouts, genome sequences, and technical labels to reinforce the "code" and "calculation" nature of the simulation.

All numerical data should use tabular figures (monospaced) to prevent UI jumping during real-time value updates.

## Layout & Spacing

The system uses a **Fluid Dashboard** model. On mobile, components are stacked or tucked into expandable bottom sheets to maximize the viewport for the hexagonal simulation canvas.

- **Simulation Canvas:** Occupies the central or top 60% of the screen.
- **Control Bar:** Fixed to the bottom of the viewport for easy thumb access.
- **Information Density:** High. Use compact 12px gaps for panels to fit more technical data without feeling cluttered.
- **Grid:** A 12-column grid is used for desktop readouts, while mobile relies on a single-column vertical stack with nested horizontal sliders for quick-access stats.

## Elevation & Depth

Hierarchy is established through **Tonal Layering** and **Subtle Glows** rather than heavy shadows.

- **Level 0 (Simulation):** The deepest layer, pure black or deep slate.
- **Level 1 (Panels):** Surface-container (#1e293b) with a 1px low-opacity border (#334155).
- **Level 2 (Overlays/Modals):** Surface-overlay (#334155) with a subtle backdrop blur (8px) to suggest a glass-like interface floating over the simulation.
- **Glow Effects:** Critical biological cells and active buttons use a soft outer glow (drop-shadow with high blur, low opacity) in their respective neon color to signify "activity."

## Shapes

The primary shape language is **Technical & Angular**. While the simulation itself is hexagonal, the UI containers use a "Soft" roundedness (0.25rem) to maintain a modern software feel without appearing too organic or "bubbly."

- **Interactive Elements:** Buttons and sliders use a 0.25rem radius.
- **Status Indicators:** Small circular pips for "active/inactive" states.
- **The Hexagon:** Reserved exclusively for the biological units within the simulation canvas. No UI buttons should be hexagonal to avoid visual confusion with simulation data.

## Components

### Simulation Controls
- **Play/Pause:** A prominent circular button with a neon-green stroke when active.
- **Speed Slider:** A slim track with a high-contrast thumb. The track "fills" with a neon-cyan gradient as the simulation speed increases.

### Technical Readouts (Stat Cards)
- **Cards:** Use a semi-transparent slate background. Headlines are JetBrains Mono. Include a small sparkline graph showing biomass trends over time.
- **Micro-stats:** Compact labels with monospaced values for FPS, Cell Count, and Energy Flux.

### Genome Editor
- **DNA Strips:** A vertical list of "genes" (chips) that can be toggled on or off.
- **Code Inputs:** Monospaced input fields for editing specific numerical values (e.g., "Mutation Rate: 0.005").

### Buttons & Inputs
- **Primary Action:** Solid neon-green background with black text for maximum contrast.
- **Secondary Action:** Ghost buttons with a slate-400 border.
- **Checkboxes:** Custom hexagonal-styled checkmarks to subtly reference the simulation geometry.

### Navigation
- **Bottom Tabs:** Minimalist icons (24px) with a glowing "dot" indicator below the active tab. Use thin-line iconography to maintain the technical aesthetic.