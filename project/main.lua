local states = {};
local player;
local ctrl;

local menu;

function clua.start()
    -- Setup window
    clua.window.setTitle('New game')

    menu = clua.create.state()
    print(menu)
end