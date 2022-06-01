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
| `clua.start` | Runs on application start | Callback | `n/a`                |
| `clua.update` | Runs every frame | Callback | deltaTime [`double`] |
| `clua.draw` | Runs every frame after `clua.update` | Callback | `n/a` |
| `clua.destroy` | Runs on application end | Callback | `n/a` |

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
| Command                 | Description            | Usage  | Arguments                     |
|-------------------------|------------------------|--------|-------------------------------|
| `clua.window.setWidth`  | Sets new window width  | Method | width [`int`]                 |
| `clua.window.setHeight` | Sets new window height | Method | height [`int`]                |
| `clua.window.setTitle`  | Sets new window title  | Method | title [`string`]              |
| `clua.window.setSize` | Sets new window size | Method | width [`int`], height [`int`] |

#### Usage
_main.lua_
```lua
-- Runs before application starts
function clua.start()
    -- Set window size
    clua.window.setWidth(800)
    clua.window.setHeight(600)
    -- Possible shorthand
    clua.window.setSize(800, 600)
    -- Set window title
    clua.window.setTitle('My new game')
end
```

### States
Application states could also be described as scenes and rooms in context or other game engines (e.g. Unity)
For Clua application to run, you need to create at least one state. Each state then has pre-defined custom camera.

| Command               | Description                                                                                    | Usage   | Arguments                               |
|-----------------------|------------------------------------------------------------------------------------------------|---------|-----------------------------------------|
| `clua.state.create`   | Creates a new state in Clua, and returns it as Lua table                                       | Method  | `n/a`                                   |
| `clua.state.destroy`  | Destroys a state                                                                               | Method  | state returned from `clua.state.create` |
| `clua.state.freeze`   | Sets current state as frozen. While frozen, `update` methods on state and objects will not run | Method  | `n/a`                                   |
| `clua.state.unfreeze` | Sets current state as unfrozen | Method | `n/a`                                   |

The returned table exposes this API:

| Command              | Description                                                      | Usage | Arguments            |
|----------------------|------------------------------------------------------------------|-------|----------------------|
| `state.setAsDefault` | Sets state as default state in Clua context                      | Method | `n/a` |
| `state.setAsActive`  | Sets state as active state                                       | Method | `n/a` |
| `state.start`        | Runs on state creation                                           | Callback | `n/a` |
| `state.update`       | Runs in the beginning of frame if state is active and not frozen | Callback | `n/a` |
| `state.draw`         | Runs in the end of frame if state is active                      | Callback | `n/a` |
| `state.destroy`      | Runs on state deletion                                           | Callback | `n/a` |


#### Game loop events
The game loop runs methods on state as needed. The order of operations is this:

- Init
  - User-defined `start()` method for state runs
  - User-defined `start()` method for objects belonging to state runs (more on objects later)
- Update
  - User-defined `update(dt)` method for state runs
  - User-defined `update(dt)` method for objects belonging to state runs (more on objects later)
- Draw
    - User-defined `draw()` method for state runs
    - User-defined `draw()` method for objects belonging to state runs (more on objects later)
- Destroy
  - User-defined `destroy()` method for objects belonging to state runs (more on objects later)
  - User-defined `destroy()` method for state runs
  
These callbacks run automatically.

#### Usage
**main.lua**
```lua
states = {}

function createMenuState()
    local state = clua.state.create()

    state.start = function(self)
        -- Create game objects
    end
    state.update = function(deltaTime) 
        -- Change state
        if (condition) then
            states.game.setAsActive()
        end
    end
    
    return state
end
function createGameState()
    return clua.state.create()
end

function clua.start()
    -- Create states
    states.menu = createMenuState()
    states.game = createGameState()
    -- Set default state
    states.menu.setAsDefault()
end
```
