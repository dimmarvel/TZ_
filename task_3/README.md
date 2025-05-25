### Пять самых активных устройств
```SQL
SELECT
    device,
    COUNT(*) AS logins
FROM players
GROUP BY device
ORDER BY logins DESC
LIMIT 5;          -- TOP 5
```

### Среднее число логинов в день за последние 7 дней
```SQL
SELECT
    AVG(daily_cnt) AS avg_logins_per_day
FROM (
    SELECT
        DATE(login_time) AS day,
        COUNT(*) AS daily_cnt
    FROM players
    WHERE login_time >= CURRENT_DATE - INTERVAL 6 DAY   -- 7дней включая сегодня
      AND login_time  < CURRENT_DATE + INTERVAL 1 DAY
    GROUP BY DATE(login_time)
) AS per_day;
```