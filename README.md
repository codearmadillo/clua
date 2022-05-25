# Clua
Description...

## Documentation
To access Clua functionalities, the API exposes a global `clua` object containing methods and/or properties.

### Lexicon
| Keyword | Description                | Usage |
| ------- |----------------------------| ----- |
| Callback | A method from Clua backend | Used to hook up to a specific behaviour e.g. keyboard events |
| Method | A method called from Lua code | Provides a specific functionality from backend Clua code |

### Runtime
| Command | Description | Usage | Arguments            |
|---------|-------------|-------|----------------------|
| `clua.start` | Runs on application start | Callback | n/a                  |
| `clua.update` | Runs every frame | Callback | deltaTime [`double`] |

#### Usage
_main.lua_
```lua
-- Runs before application starts
function clua.start()
    -- Set window size
    clua.window.setWidth(800)
end

-- Runs every frame
function clua.update(deltaTime)
    player.move(player.speed * deltaTime)
end
```

### Window
| Command                 | Description            | Usage  | Arguments        |
|-------------------------|------------------------|--------|------------------|
| `clua.window.setWidth`  | Sets new window width  | Method | width [`int`]    |
| `clua.window.setHeight` | Sets new window height | Method | height [`int`]   |
| `clua.window.setTitle`  | Sets new window title  | Method | title [`string`] |


#### Usage
_main.lua_
```lua
-- Runs before application starts
function clua.start()
    -- Set window size
    clua.window.setWidth(800)
    clua.window.setHeight(600)
    clua.window.setTitle('My new game')
end
```