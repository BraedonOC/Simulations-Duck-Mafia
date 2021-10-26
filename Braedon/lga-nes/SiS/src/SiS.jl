using DataStructures
using Random


struct event
    at
    type
    count
end


S = 100  # this might be wrong)
s = 40

function geometric(p)
    x = rand(1)[1] 
    return floor((1 - p) ^ (x - 1) * p)
end

function exponential(λ)
    x = rand(1)[1]
    return λ * exp(-λ*x)
end

eventList = MutableLinkedList()
t = 0 
τ = 1000

inventory = S
onOrder = 0

backorderCount = 0
orderCount = 0

firstEvent = event(exp(2/3), "DemandEvent", -1)
append!(eventList, firstEvent)

function get_at(target_event)
    return target_event.at 
end

while length(eventList) > 0
    sort!(collect(eventList), by = x->x.at)   # should sort by at 
    currentEvent = getindex(eventList, 1)
    t = currentEvent.at
    popfirst!(eventList)
    if (currentEvent.type == "ReviewEvent" )    # PROCESS EVENT 
        nextReview(t + 7, "")
        nextReview.at = t + 7
        if nextReview.at < τ
            nextReview.type = "ReviewEvent"
            append!(eventList, nextReview)
        end
        # re order?
        if inventory + onOrder <= s 
            newDelivery = event(t + exp(7), "DeliveryEvent", S - (inventory + onOrder))
            global onOrder += newDelivery.count  
            append!(eventList, newDelivery)
            global orderCount += 1
        end
    elseif currentEvent.type == "DeliveryEvent"   # PROCESS EVENT
        global onOrder -= currentEvent.count
        global inventory += currentEvent.count

    elseif currentEvent.type == "DemandEvent"   # PROCESS EVENT
        # schedule the next demand
        next_t = t + exp(2/3)
        if next_t < τ
            nextDemand = event(next_t, "DemandEvent", -1)
            append!(eventList, nextDemand)
        end
        purchased = geometric(3/4)
        if purchased > 0
            if inventory >= purchased 
                global inventory -= purchased
            else 
                if inventory > 0 
                    purchased -= inventory 
                    global inventory = 0
                end 
                global inventory -= purchased 
                global backorderCount += purchased
            end
        end
    end
    if inventory < S
        if inventory < 0 
            global backorderCount += abs(inventory)
        end
        global orderCount += 1 
        global t += exponential(7)
    end
    println(t, " ", orderCount, " ", backorderCount, " ")
end

