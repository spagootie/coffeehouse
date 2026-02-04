INSERT INTO users (username, display_name) VALUES
('alice', 'Alice'),
('bob', 'Bob'),
('charlie', 'Charlie');

INSERT INTO conversations () VALUES ();
INSERT INTO conversations () VALUES ();

INSERT INTO conversation_participants (conversation_id, user_id) VALUES
(1, 1),
(1, 2),
(2, 2),
(2, 3);

INSERT INTO messages (conversation_id, sender_id, content) VALUES
(1, 1, 'Hey Bob'),
(1, 2, 'Hey Alice'),
(2, 2, 'Hi Charlie'),
(2, 3, 'Hey!');

