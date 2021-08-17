-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Crime Date: July 28, 2020 (dd/mm/yy) (28/07/20)
-- Chamberlin Street


-- List all crime reports taking place that day and street
SELECT id, description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2020 AND street = "Chamberlin Street"
-- Info gained:
-- Theft took place at 10:15am at Chamberlin Street Courthouse.
-- Interview with three witnesses, all interview transcripts mention courthouse.


-- Find the three witnesses
SELECT id, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2020
-- Witness ID: 161, 162, 163
-- 161: Within 10 minutes of theft, saw thief enter courthouse parking lot driving away. Check security for leaving cars.
-- 162: Saw thief WITHDRAWING money from ATM on "Fifer Street" earlier that morning
-- 163: As thief left, they called someone. Mentioned they would take earliest flight OUT of fiftyville tomorrow. Asked person to purchase
-- NOTE: Thief did not EXPLICITLY say earliest flight out of fiftyville would take place from Fiftyville Airport.


-- 163.0: Check flights taking place the next day
SELECT full_name, hour, minute FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE day = 29 AND month = 7 AND year = 2020 AND city = "Fiftyville" ORDER BY hour ASC
-- Found list of flights departing from Fiftyville Regional Airport.


-- 162.0: Check atm withdraw transactions on morning of theft
SELECT account_number, amount FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw"
-- Basically, a massive list of withdraw transactions


-- 162.1: Check atm transactions of possible suspects
SELECT account_number FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE person_id IN (SELECT id FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY id ASC)
INTERSECT
SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw"
-- Results list the account numbers of suspects who've made a transaction that morning


-- 162.2: Find the name's of those suspects who made a morning transaction.
SELECT name FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE id IN (SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE person_id IN (SELECT id FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY id ASC)
INTERSECT
SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw"))
ORDER BY name ASC
-- Narrowed the suspects down to 4: Ernest, Russell, Elizabeth, Danielle


-- 162.3: Cross-reference suspect numbers with those who've taken a call
SELECT name, id, phone_number FROM people
WHERE phone_number IN (SELECT phone_number FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE id IN (SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM bank_accounts
JOIN people ON bank_accounts.person_id = people.id
WHERE person_id IN (SELECT id FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY id ASC)
INTERSECT
SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND transaction_type = "withdraw"))
INTERSECT
SELECT caller FROM phone_calls
WHERE caller IN (SELECT phone_number FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY name ASC)
AND day = 28 AND month = 7 AND year = 2020)
-- Suspects have been narrowed down to either Ernest or Russell


-- 161.0: Check cars that have left courthouse within 10 minutes
SELECT license_plate, hour, minute FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC
-- Get a list of license plates for possible suspects. 8 cars total.


-- 161.1: Find the people who have those license plate ID's.
SELECT license_plate, name, phone_number, passport_number FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY name ASC
-- Essentially a list of suspects. Lists additional info on the possible suspects.


-- 161.2: Check if the suspects have made a phone call day of crime (phone number)
SELECT id, caller, receiver, duration FROM phone_calls
WHERE caller IN (SELECT phone_number FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY name ASC)
AND day = 28 AND month = 7 AND year = 2020 AND duration <= 60
-- Pretty thicc list of calls made that day.
-- Narrowed it down to calls less than a minute, check the receivers purchase history.


-- 161.3: Check the flight history of Ernest and Russell using their ID's(passport number)
SELECT flight_id, origin_airport_id, destination_airport_id, seat, passport_number FROM passengers
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.origin_airport_id = airports.id
WHERE passport_number IN (SELECT passport_number FROM people
WHERE id IN (514354, 686048))
AND day = 29 AND month = 7
-- On July 29, 2020, 2 flights were made.
-- Russell passport: 359 number: 770 Russell flew to Boston. Russell called Philip
-- Ernest passport: 577 number: 367 Ernest flew to London. Ernest called Berthold


-- 161.4: Improved suspect call search with new information
SELECT name, phone_number FROM people
WHERE phone_number IN (SELECT receiver FROM phone_calls
WHERE caller IN (SELECT phone_number FROM people
WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = "exit"
ORDER BY hour DESC, minute ASC) ORDER BY name ASC)
AND day = 28 AND month = 7 AND year = 2020 AND duration <= 60 AND caller IN ("(367) 555-5533", "(770) 555-1861")
INTERSECT
SELECT phone_number FROM people)
-- Found the two receivers. Philip and Bertolt, phone number of 725, and 375 respectively.


-- 161.5: Check atm transactions of philip and bertolt
SELECT atm_location, transaction_type, amount FROM atm_transactions
WHERE account_number IN (SELECT account_number FROM bank_accounts
WHERE person_id IN (SELECT id FROM people WHERE phone_number IN ("(367) 555-5533", "(770) 555-1861")
ORDER BY name ASC))