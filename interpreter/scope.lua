require "interpreter/env"

function scope(name)

    local scope = {
        name = name,
        instructions = {},
    }

    openScopes = openScopes + 1
    onScope    = true

    function scope:newInstruction(instruction)

        table.insert(self.instructions, instruction)
    end

    function scope:execute()
        
        for k, v in next, self.instructions do
            
            v()
        end
    end

    return scope
end