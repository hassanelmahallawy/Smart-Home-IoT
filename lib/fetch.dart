import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_database/ui/firebase_animated_list.dart';
import 'package:flutter/material.dart';

class FetchData extends StatefulWidget {
  const FetchData({Key? key}) : super(key: key);

  @override
  State<FetchData> createState() => _FetchDataState();
}

class _FetchDataState extends State<FetchData> {
  late DatabaseReference dbRef;
  bool doorStatus = false;
  bool windowStatus = false;

  @override
  void initState() {
    super.initState();
    dbRef = FirebaseDatabase.instance.reference();
  }

  Widget listItem({required Map data, required String key}) {
    return Card(
      elevation: 4,
      margin: const EdgeInsets.all(10),
      color: Color(0xFFE9E9E9),
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            ListTile(
              title: Text(
                'SMART HOME SYSTEM',
                style: TextStyle(
                  fontSize: 18,
                  fontWeight: FontWeight.bold,
                  color: Colors.orange,
                ),
              ),
              subtitle: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  for (String field in data.keys)
                    Padding(
                      padding: const EdgeInsets.symmetric(vertical: 4),
                      child: Text(
                        '$field: ${data[field]}',
                        style: TextStyle(
                          fontSize: 16,
                          color: Colors.black87,
                        ),
                      ),
                    ),
                ],
              ),
            ),
            const SizedBox(height: 20),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceAround,
              children: [
                _buildStatusToggle('Door', doorStatus, (newValue) {
                  setState(() {
                    doorStatus = newValue;
                  });
                  updateDoorStatus(key, newValue);
                }),
                _buildStatusToggle('Window', windowStatus, (newValue) {
                  setState(() {
                    windowStatus = newValue;
                  });
                  updateWindowStatus(key, newValue);
                }),
              ],
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildStatusToggle(
      String label, bool status, Function(bool) onChanged) {
    return Column(
      children: [
        Text(
          label,
          style: TextStyle(
            fontSize: 16,
            color: Colors.black87,
          ),
        ),
        const SizedBox(height: 5),
        Switch(
          activeColor: Colors.orange,
          value: status,
          onChanged: onChanged,
        ),
      ],
    );
  }

  void updateDoorStatus(String key, bool newStatus) {
    String status = newStatus ? 'Opened' : 'Closed';
    dbRef.child(key).child("Actuators").child("door").set(status);
  }

  void updateWindowStatus(String key, bool newStatus) {
    String status = newStatus ? 'Opened' : 'Closed';
    dbRef.child(key).child("Actuators").child("Window").set(status);
  }

  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: Scaffold(
        appBar: AppBar(
          title: Text(
            'Show Data',
            style: TextStyle(
              fontSize: 24,
              fontWeight: FontWeight.bold,
              color: Colors.white,
            ),
          ),
          centerTitle: true,
          backgroundColor: Colors.orange,
        ),
        body: Column(
          children: [
            Expanded(
              child: Container(
                color: Color(0xFFF5F5F5),
                child: FirebaseAnimatedList(
                  query: dbRef,
                  itemBuilder: (
                      BuildContext context,
                      DataSnapshot snapshot,
                      Animation<double> animation,
                      int index,
                      ) {
                    Map data = snapshot.value as Map;
                    String key = snapshot.key!;

                    return listItem(data: data, key: key);
                  },
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}