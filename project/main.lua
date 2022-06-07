require "state-menu"

local states = {};

function clua.start()
    -- Setup window
    clua.window.setTitle('New game test')
    -- Create states
    states.menu = createMenuState()
end