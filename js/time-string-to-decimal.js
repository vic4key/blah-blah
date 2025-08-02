let [h, m, s] = "08:45:38".split(":").map(v => ~~v)
let tmp_time = h * 10000 + m * 100 + s; // 84538
let max_time = 94500; // 09:45:00