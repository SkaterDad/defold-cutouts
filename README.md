# defold-cutouts
Defold native extension which provides display cutout information.

## Installation
Add a link to this extension in your Defold `game.project`.

```
https://github.com/SkaterDad/defold-cutouts/archive/master.zip
```

## Usage
```lua
  if cutouts then
      local left, right, top, bottom = cutouts.get_safe_offsets()
  end
```
