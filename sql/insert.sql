-- -----------------------------
-- Users
-- -----------------------------
INSERT INTO users (username, display_name)
VALUES
('alice', 'Alice A'),
('bob', 'Bob B'),
('carol', 'Carol C'),
('dave', 'Dave D'),
('eve', 'Eve E'),
('frank', 'Frank F'),
('grace', 'Grace G'),
('heidi', 'Heidi H'),
('ivan', 'Ivan I'),
('judy', 'Judy J');

-- -----------------------------
-- Conversations
-- -----------------------------
INSERT INTO conversations (created_at)
VALUES
(NOW()), (NOW()), (NOW()), (NOW()), (NOW());

-- -----------------------------
-- Conversation Participants
-- -----------------------------
-- convo 1
INSERT INTO conversation_participants (conversation_id, user_id)
VALUES (1,1),(1,2),(1,3);

-- convo 2
INSERT INTO conversation_participants (conversation_id, user_id)
VALUES (2,2),(2,3),(2,4),(2,5);

-- convo 3
INSERT INTO conversation_participants (conversation_id, user_id)
VALUES (3,1),(3,5),(3,6);

-- convo 4
INSERT INTO conversation_participants (conversation_id, user_id)
VALUES (4,7),(4,8),(4,9),(4,10);

-- convo 5
INSERT INTO conversation_participants (conversation_id, user_id)
VALUES (5,1),(5,2),(5,10);

-- -----------------------------
-- Messages
-- -----------------------------
-- conversation 1
INSERT INTO messages (conversation_id, sender_id, content, sent_at)
VALUES
(1,1,'Hello everyone!', NOW()),
(1,2,'Hi Alice!', NOW()),
(1,3,'Hey guys!', NOW()),
(1,1,'How are you?', NOW()),
(1,2,'Doing great!', NOW());

-- conversation 2
INSERT INTO messages (conversation_id, sender_id, content, sent_at)
VALUES
(2,2,'Welcome to convo 2', NOW()),
(2,3,'Thanks Bob!', NOW()),
(2,4,'Hello!', NOW()),
(2,5,'Hi all!', NOW());

-- conversation 3
INSERT INTO messages (conversation_id, sender_id, content, sent_at)
VALUES
(3,1,'Hey group 3', NOW()),
(3,5,'Hi Alice!', NOW()),
(3,6,'Hello!', NOW());

-- conversation 4
INSERT INTO messages (conversation_id, sender_id, content, sent_at)
VALUES
(4,7,'Group 4 starts', NOW()),
(4,8,'Hi everyone', NOW()),
(4,9,'Hello!', NOW()),
(4,10,'Hey there!', NOW()),
(4,7,'How is everyone?', NOW());

-- conversation 5
INSERT INTO messages (conversation_id, sender_id, content, sent_at)
VALUES
(5,1,'Convo 5 kickoff', NOW()),
(5,2,'Hi!', NOW()),
(5,10,'Hello from Judy', NOW());

