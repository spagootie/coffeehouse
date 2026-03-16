USE chat_app;

INSERT INTO users (username, display_name) VALUES
('alice','Alice'), ('bob','Bob'), ('carol','Carol');

INSERT INTO conversations (title) VALUES ('General'), ('Project');

INSERT INTO participants (conversation_id, user_id) VALUES
(1,1),(1,2),(1,3),(2,1),(2,2);

INSERT INTO messages (conversation_id, sender_id, content) VALUES
(1,1,'Hi everyone'),
(1,2,'Hi Alice'),
(2,1,'Project kickoff at 2pm');
